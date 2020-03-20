#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QApplication>
#include <QMenuBar>
#include <QLayout>
#include <QTcpSocket>

#include "flashlightwidget.h"
#include "connectiondialog.h"
#include "pluginmanager.h"
#include "core_interface.h"

#ifdef QT_DEBUG
#include <QCheckBox>
#include <QComboBox>
#include <QPushButton>
#endif

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void translate(const QByteArray &message);
#ifdef QT_DEBUG
    void testSwitchColor(int color);
    void testSwitchPower(const bool power);
#endif
    void switchColor(QColor color);
    void setFlashlightPower(bool power);


private:
    QMenu *_fileMenu;
    QAction *_newAct;
    QAction *_exitAct;
    QMenu *_helpMenu;
    QAction *helpAct;

    QWidget *_centralWidget;
    QVBoxLayout *_verticalLayout;
    FlashlightWidget *_flashlightWidget;

    ConnectionDialog *_dialog;

    void createActions();
    void createMenus();
    void createWidgets();


#ifdef QT_DEBUG
    QCheckBox *_testButton;
    QComboBox *_testColor;
#endif
};
Q_DECLARE_METATYPE(Color)           //needs for cast Color-type from QVariant
#endif // MAINWINDOW_H
