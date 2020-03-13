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
    _dialog->exec();
   // connect(_dialog, SIGNAL());

    _centralWidget = new QWidget(this);
    setCentralWidget(_centralWidget);

    _verticalLayout = new QVBoxLayout(this);
    _centralWidget->setLayout(_verticalLayout);

    _testButton = new QCheckBox("Switch", this);
    _testButton->setCheckState(Qt::CheckState::Unchecked);
    _flashlightWidget->setState(false);


    _testColor = new QComboBox(this);
    _testColor->addItem("White");
    _testColor->addItem("Red");
    _testColor->addItem("Green");
    _testColor->addItem("Blue");
    connect(_testColor, SIGNAL(currentIndexChanged(int)), this, SLOT(switchColor(int)));

    connect(_dialog, SIGNAL(connectError(bool)), _flashlightWidget, SLOT(setError(bool)));
    _flashlightWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    _verticalLayout->addWidget(_flashlightWidget);
    _verticalLayout->addWidget(_testButton);
    _verticalLayout->addWidget(_testColor);

    connect(_testButton, SIGNAL(toggled(bool)), _flashlightWidget, SLOT(setState(bool)));
}

MainWindow::~MainWindow()
{
}

void MainWindow::switchColor(int color)
{
    switch (color) {
    case 0:
        _flashlightWidget->setBackgroundColor(0xFFFFFFFF);
    break;
    case 1:
        _flashlightWidget->setBackgroundColor(0xFFFF0000);
    break;
    case 2:
        _flashlightWidget->setBackgroundColor(0xFF00FF00);
    break;
    case 3:
        _flashlightWidget->setBackgroundColor(0xFF0000FF);
    break;
    default:
        _flashlightWidget->setBackgroundColor(0xFFFF0000);
    }
    _dialog->exec();
}

