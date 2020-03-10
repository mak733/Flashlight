#include "mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{

    _centralWidget = new QWidget(this);
    setCentralWidget(_centralWidget);

    _verticalLayout = new QVBoxLayout(this);
    _centralWidget->setLayout(_verticalLayout);

    _testButton = new QPushButton("Switch", this);
    _verticalLayout->addWidget(_testButton);

}

MainWindow::~MainWindow()
{
}

