#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLayout>
#include <QCheckBox>

#include "svgview.h"



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void switchLight();

private:

    QWidget *_centralWidget;

    QVBoxLayout *_verticalLayout;
    QCheckBox *_testButton;
    SvgView *m_view;
};
#endif // MAINWINDOW_H
