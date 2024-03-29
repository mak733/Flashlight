﻿#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QApplication>
#include <QMenuBar>
#include <QLayout>
#include <QTcpSocket>

#include "flashlightwidget.h"
#include "connectiondialog.h"
#include "plugins/pluginmanager.h"
#include "core_interface.h"

#include <QProcessEnvironment>
#include <QCheckBox>
#include <QComboBox>
#include <QPushButton>
#include <QMessageBox>

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr,
                        const char *ip = "127.0.0.1",
                        const char *port = "9999");

    ~MainWindow();

public slots:

    void slotTranslate(const QByteArray &message);
    void slotCreateTestWidgets();
    void slotTestSwitchColor(int color);
    void slotTestSwitchPower(const bool power);
    void slotSwitchColor(QColor color);
    void slotSetFlashlightPower(bool power);
    void slotShowInfo();

private:
    void createActions();
    void createMenus();
    void createWidgets();
    void testWidgets();

private:
    QMenu *_fileMenu;
    QAction *_connectionSettingsAct;
    QAction *_exitAct;
    QMenu *_helpMenu;
    QAction *_helpAct;

    QWidget *_centralWidget;
    QVBoxLayout *_verticalLayout;
    FlashlightWidget *_flashlightWidget;

    ConnectionDialog *_dialog;

#ifdef QT_DEBUG
    QCheckBox *_testButton;
    QComboBox *_testColor;
#endif

};

Q_DECLARE_METATYPE(Color)           //needs for cast Color-type from QVariant
#endif // MAINWINDOW_H
