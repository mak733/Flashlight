/****************************************************************************
**
****************************************************************************/

#ifndef CLIENT_H
#define CLIENT_H

#include <QDataStream>
#include <QDialog>
#include <QTcpSocket>
#include <QDebug>

#include "header.h"

QT_BEGIN_NAMESPACE
class QComboBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QTcpSocket;
class StreamInterface;

QT_END_NAMESPACE

//! [0]
class ConnectionDialog : public QDialog {
Q_OBJECT
public:
    explicit ConnectionDialog(QWidget *parent = nullptr, QString ip = "127.0.0.1", QString port = "9999");
    ~ConnectionDialog();
    void testMessage(QByteArray message);

signals:
    void connectError(bool state);
    void readMessage(QByteArray message);

private slots:
    void requestNewConnection();
    void displayError(QAbstractSocket::SocketError socketError);
    void slotReadyRead();
    void slotConnected();

private:
    QLineEdit *_hostLineEdit = nullptr;
    QLineEdit *_portLineEdit = nullptr;
    QRegExp _ipRegex;
    QLabel *_statusLabel = nullptr;
    QPushButton *_connectButton = nullptr;
    QTcpSocket *_tcpSocket = nullptr;

};
//! [0]

#endif
