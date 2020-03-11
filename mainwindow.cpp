#include "mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_view(new SvgView(":/image/flashlight.svg", ":/image/light.svg", this))
{

    _centralWidget = new QWidget(this);
    setCentralWidget(_centralWidget);

    _verticalLayout = new QVBoxLayout(this);
    _centralWidget->setLayout(_verticalLayout);

    _testButton = new QCheckBox("Switch", this);
    _testButton->setCheckState(Qt::CheckState::Unchecked);

    _verticalLayout->addWidget(m_view);
    _verticalLayout->addWidget(_testButton);

    connect(_testButton, SIGNAL(stateChanged(int)), m_view, SLOT(setLightState(int)));
}

MainWindow::~MainWindow()
{
}

void MainWindow::switchLight()
{

}

