#include "mainwindow.h"
#include <iostream>
#include <chrono>
#include <thread>
using namespace std::chrono;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , _flashlightWidget(new FlashlightWidget(":/image/flashlight.svg"
                                             ,":/image/light.svg"
                                             ,":/image/connection_error.svg"
                                             ,this))
{
    _dialog = new ConnectionDialog(this);
    createActions();
    createMenus();
    createWidgets();

    if(_dialog->result() == QDialog::Rejected)
        _flashlightWidget->setError(true);

    connect(_dialog, SIGNAL(readMessage(QByteArray)), this, SLOT(slotTranslate(QByteArray)));
    if(QProcessEnvironment::systemEnvironment().value("SELFTEST") != "true")
        _dialog->exec();
}

MainWindow::~MainWindow()
{
    delete _flashlightWidget;
    delete _dialog;
    qDebug() << "quit";
}

void MainWindow::createActions()
{

    if(QProcessEnvironment::systemEnvironment().value("SELFTEST") != "true")
    {
        _connectionSettingsAct = new QAction(tr("&Connection settings"), this);
        _connectionSettingsAct->setShortcuts(QKeySequence::Preferences);
        _connectionSettingsAct->setStatusTip(tr("Connection settings"));
        connect(_connectionSettingsAct, &QAction::triggered, _dialog, &ConnectionDialog::exec);
    }

    _exitAct = new QAction(tr("&Quit"), this);
    _exitAct->setShortcuts(QKeySequence::Quit);
    _exitAct->setStatusTip(tr("Quit"));
    connect(_exitAct, &QAction::triggered, qApp, QApplication::quit);

    _helpAct = new QAction(tr("&Help"), this);
    _helpAct->setShortcuts(QKeySequence::HelpContents);
    _helpAct->setStatusTip(tr("Create a new file"));
    ///TODO: manual-widget
    //connect(_helpAct, &QAction::triggered, this, SLOT(slotShowInfo()));
}

void MainWindow::createMenus()
{
    _fileMenu = menuBar()->addMenu(tr("&File"));

    if(QProcessEnvironment::systemEnvironment().value("SELFTEST") != "true")
        _fileMenu->addAction(_connectionSettingsAct);
    _fileMenu->addAction(_exitAct);

    _helpMenu = menuBar()->addMenu(tr("&Help"));
    _helpMenu->addAction(_helpAct);
}

void MainWindow::createWidgets()
{
    _centralWidget = new QWidget(this);
    setCentralWidget(_centralWidget);

    _verticalLayout = new QVBoxLayout(this);
    _centralWidget->setLayout(_verticalLayout);

    connect(_dialog, SIGNAL(connectError(bool)), _flashlightWidget, SLOT(setError(bool)));
    _flashlightWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    _verticalLayout->addWidget(_flashlightWidget);
    if(QProcessEnvironment::systemEnvironment().value("SELFTEST") == "true")
        slotCreateTestWidgets();

}


void MainWindow::slotTranslate(const QByteArray &message)
{
    CoreInterface *core = PluginManager::instance()->getPlugin(Protocol::TLV, message.at(0));
    if(!core)
    {
        qDebug() << "Can't find translation plugin. Type unknown message:" << message.at(0);
        return;
    }
    switch (core->codogrammType()) {
    case Command::Type::On:
    case Command::Type::Off:{
        qDebug() << "Switch power:" << core->value("питание", message).toBool();
        slotSetFlashlightPower(core->value("питание", message).toBool());
        break;
    }
    case Command::Type::Color:{
        QColor newColor;
        Color color = core->value("цвет", message).value<Color>();
        newColor.setRgb(color.red, color.green, color.blue);
        qDebug() << "Switch color:" << newColor;
        slotSwitchColor(newColor);
        break;
    }
    default:{
        qDebug() << "Income message " << core->name() << "nothing to do";
        break;
    }
    }
}

void MainWindow::slotSwitchColor(const QColor color)
{
    _flashlightWidget->setBackgroundColor(color);
}

void MainWindow::slotSetFlashlightPower(const bool power)
{
    _flashlightWidget->setState(power);

}

void MainWindow::slotShowInfo()
{
    QMessageBox::information(this, tr("Flashlight Client"),
                             tr("The connection was refused by the peer. "
                                "Make sure the flashlight server is running, "
                                "and check that the host name and port "
                                "settings are correct."));
}

void MainWindow::slotCreateTestWidgets()
{
    _testButton = new QCheckBox("Switch", this);
    _testButton->setCheckState(Qt::CheckState::Unchecked);
    _flashlightWidget->setState(false);

    _testColor = new QComboBox(this);
    _testColor->addItem("White");
    _testColor->addItem("Red");
    _testColor->addItem("Green");
    _testColor->addItem("Blue");
    connect(_testColor, SIGNAL(currentIndexChanged(int)), this, SLOT(slotTestSwitchColor(int)));

    _verticalLayout->addWidget(_testButton);
    _verticalLayout->addWidget(_testColor);

    connect(_testButton, SIGNAL(toggled(bool)), this, SLOT(slotTestSwitchPower(bool)));
}

void MainWindow::slotTestSwitchColor(quint32 color)
{
    static const char colorRedMessage[] = {
        0x20, 0x00, 0x03, static_cast<char>(0xff), 0x00, 0x00
    };
    static const char colorGreenMessage[] = {
        0x20, 0x00, 0x03, 0x00, static_cast<char>(0xff), 0x00
    };
    static const char colorBlueMessage[] = {
        0x20, 0x00, 0x03, 0x00, 0x00, static_cast<char>(0xff)
    };
    static const char colorWhiteMessage[] = {
        0x20, 0x00, 0x03, static_cast<char>(0xff), static_cast<char>(0xff), static_cast<char>(0xff)
    };
    static const char errorMessage[] = {
        0x43, 0x12, 0x43, 0x00, 0x00, static_cast<char>(0xff)
    };

    switch (color) {
    case 0:
        _dialog->testMessage(QByteArray::fromRawData(colorWhiteMessage, sizeof(colorWhiteMessage)));
        break;
    case 1:
        _dialog->testMessage(QByteArray::fromRawData(colorRedMessage, sizeof(colorRedMessage)));
        break;
    case 2:
        _dialog->testMessage(QByteArray::fromRawData(colorGreenMessage, sizeof(colorGreenMessage)));
        break;
    case 3:
        _dialog->testMessage(QByteArray::fromRawData(colorBlueMessage, sizeof(colorBlueMessage)));
        break;
    default:
        _flashlightWidget->setBackgroundColor(0xFFFF0000);
    }
}

void MainWindow::slotTestSwitchPower(const bool power)
{
    static const char onMessage[] = {
        0x12, 0x00, 0x00
    };
    static const char offMessage[] = {
        0x13, 0x00, 0x00
    };
    if(power)
        _dialog->testMessage(QByteArray::fromRawData(onMessage, sizeof(onMessage)));
    else
        _dialog->testMessage(QByteArray::fromRawData(offMessage, sizeof(offMessage)));

}
