#include "mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_view(new FlashlightWidget(":/image/flashlight.svg", ":/image/light.svg", this))
{

    _centralWidget = new QWidget(this);
    setCentralWidget(_centralWidget);

    _verticalLayout = new QVBoxLayout(this);
    _centralWidget->setLayout(_verticalLayout);

    _testButton = new QCheckBox("Switch", this);
    _testButton->setCheckState(Qt::CheckState::Unchecked);
    m_view->setState(false);

    _testColor = new QComboBox(this);
    _testColor->addItem("White");
    _testColor->addItem("Red");
    _testColor->addItem("Green");
    _testColor->addItem("Blue");
    connect(_testColor, SIGNAL(currentIndexChanged(int)), this, SLOT(switchColor(int)));

    _verticalLayout->addWidget(m_view);
    _verticalLayout->addWidget(_testButton);
    _verticalLayout->addWidget(_testColor);

    connect(_testButton, SIGNAL(toggled(bool)), m_view, SLOT(setState(bool)));
}

MainWindow::~MainWindow()
{
}

void MainWindow::switchColor(int color)
{
    switch (color) {
    case 1:
        m_view->setBackgroundColor(0xFFFF0000);
    break;
    case 2:
        m_view->setBackgroundColor(0xFF00FF00);
    break;
    case 3:
        m_view->setBackgroundColor(0xFF0000FF);
    break;
    default:
        m_view->setBackgroundColor(0xFFFFFFFF);
    }
}

