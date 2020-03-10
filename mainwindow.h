#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLayout>
#include <QPushButton>

#include "svgview.h"



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private:

    QWidget *_centralWidget;

    QVBoxLayout *_verticalLayout;
    QPushButton *_testButton;
    SvgView *m_view;
};
#endif // MAINWINDOW_H
