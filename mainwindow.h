#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QApplication>
#include <QMenuBar>
#include <QLayout>
#include <QCheckBox>
#include <QComboBox>
#include <QPushButton>

#include <QTcpSocket>



#include "flashlightwidget.h"
#include "connectiondialog.h"



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void switchColor(int color);

private:


    QMenu *_fileMenu;
    QAction *newAct;
    QAction *exitAct;
    QMenu *_helpMenu;
    QAction *helpAct;

    QWidget *_centralWidget;
    QVBoxLayout *_verticalLayout;
    FlashlightWidget *_flashlightWidget;
    QCheckBox *_testButton;
    QComboBox *_testColor;

    ConnectionDialog *_dialog;
    QTcpSocket *_socket;

    void createActions();
    void createMenus();
    void createWidgets();
};
#endif // MAINWINDOW_H
