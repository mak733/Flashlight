#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLayout>

#include <QCheckBox>
#include <QComboBox>

#include "flashlightwidget.h"



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void switchColor(int color);

private:

    QWidget *_centralWidget;

    QVBoxLayout *_verticalLayout;
    FlashlightWidget *m_view;


    QCheckBox *_testButton;
    QComboBox *_testColor;
};
#endif // MAINWINDOW_H
