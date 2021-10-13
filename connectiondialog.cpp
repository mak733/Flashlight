#include <QtWidgets>
#include <QtNetwork>

#include "connectiondialog.h"
#include "stream_interface.h"

ConnectionDialog::ConnectionDialog(QWidget *parent, QString ip, QString port)
        : QDialog(parent), _hostLineEdit(new QLineEdit), _portLineEdit(new QLineEdit), _tcpSocket(new QTcpSocket) {
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    QString ipRange = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
    _ipRegex = QRegExp("^" + ipRange
                       + "\\." + ipRange
                       + "\\." + ipRange
                       + "\\." + ipRange + "$");

    QRegExpValidator *ipValidator = new QRegExpValidator(_ipRegex, this);
    _hostLineEdit->setValidator(ipValidator);
    int pos = 0;
    if (ipValidator->validate(ip, pos))
        _hostLineEdit->setText(ip);
    else {
        qDebug() << "Incorrect IP" << ip << ", will set IP to 127.0.0.1";
        _hostLineEdit->setText("127.0.0.1");
    }


    QIntValidator *portValidator = new QIntValidator(1, 65535, this);
    _portLineEdit->setValidator(portValidator);
    if (portValidator->validate(port, pos)) {
        _portLineEdit->setText(port);
    } else {
        qDebug() << "Incorrect port" << port << ", will set port to 9999";
        _portLineEdit->setText("9999");
    }

    auto hostLabel = new QLabel(tr("&IP-address:"));
    hostLabel->setBuddy(_hostLineEdit);
    auto portLabel = new QLabel(tr("&Port:"));
    portLabel->setBuddy(_portLineEdit);

    _statusLabel = new QLabel(tr("This examples requires that you run the "
                                 "Flashlight Server example as well."));

    _connectButton = new QPushButton("Connect", this);
    _connectButton->setDefault(true);

    auto quitButton = new QPushButton(tr("Quit"));

    auto buttonBox = new QDialogButtonBox;
    buttonBox->addButton(quitButton, QDialogButtonBox::RejectRole);
    buttonBox->addButton(_connectButton, QDialogButtonBox::AcceptRole);


    connect(_connectButton, &QAbstractButton::clicked,
            this, &ConnectionDialog::requestNewConnection);
    connect(quitButton, &QAbstractButton::clicked, this, &QWidget::close);

    connect(_tcpSocket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),
            this, &ConnectionDialog::displayError);
    connect(_tcpSocket, SIGNAL(connected()), SLOT(slotConnected()));
    connect(_tcpSocket, SIGNAL(readyRead()), SLOT(slotReadyRead()));


    QGridLayout *mainLayout = nullptr;
    if (QGuiApplication::styleHints()->showIsFullScreen() || QGuiApplication::styleHints()->showIsMaximized()) {
        auto outerVerticalLayout = new QVBoxLayout(this);
        outerVerticalLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Ignored, QSizePolicy::MinimumExpanding));
        auto outerHorizontalLayout = new QHBoxLayout;
        outerHorizontalLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Ignored));
        auto groupBox = new QGroupBox(QGuiApplication::applicationDisplayName());
        mainLayout = new QGridLayout(groupBox);
        outerHorizontalLayout->addWidget(groupBox);
        outerHorizontalLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Ignored));
        outerVerticalLayout->addLayout(outerHorizontalLayout);
        outerVerticalLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Ignored, QSizePolicy::MinimumExpanding));
    } else {
        mainLayout = new QGridLayout(this);
    }
    mainLayout->addWidget(hostLabel, 0, 0);
    mainLayout->addWidget(_hostLineEdit, 0, 1);
    mainLayout->addWidget(portLabel, 1, 0);
    mainLayout->addWidget(_portLineEdit, 1, 1);
    mainLayout->addWidget(_statusLabel, 2, 0, 1, 2);
    mainLayout->addWidget(buttonBox, 3, 0, 1, 2);

    setWindowTitle(QGuiApplication::applicationDisplayName());
    _portLineEdit->setFocus();
    setWindowTitle("Connection settings");

}

ConnectionDialog::~ConnectionDialog() {
    qDebug() << "~ConnectionDialog()";
    _tcpSocket->abort();

}


void ConnectionDialog::testMessage(QByteArray message) {
    if (QProcessEnvironment::systemEnvironment().value("SELFTEST") == "true") {
        qDebug() << "Immitate new message" << message.toHex();
        emit readMessage(message);
    }
}

void ConnectionDialog::requestNewConnection() {
    qDebug() << "Close" << _tcpSocket->peerAddress() << _tcpSocket->peerPort();
    _tcpSocket->close();
    _connectButton->setEnabled(false);                  // we must not try to connect while _tcpSocket isn't close
    _tcpSocket->connectToHost(_hostLineEdit->text(),
                              _portLineEdit->text().toInt());

}

void ConnectionDialog::displayError(QAbstractSocket::SocketError socketError) {
    switch (socketError) {
        case QAbstractSocket::RemoteHostClosedError:
            qDebug() << "The host closed the connection." << _tcpSocket->peerAddress() << _tcpSocket->peerPort();
            QMessageBox::information(this, tr("Closed the connection"),
                                     tr("The host closed the connection."));
            break;
        case QAbstractSocket::HostNotFoundError:
            qDebug() << "The host was not found" << _tcpSocket->peerAddress() << _tcpSocket->peerPort();;
            QMessageBox::information(this, tr("The host was not found"),
                                     tr("The host was not found. Please check the "
                                        "host name and port settings."));
            break;
        case QAbstractSocket::ConnectionRefusedError:
            qDebug() << "The connection was refused" << _tcpSocket->peerAddress() << _tcpSocket->peerPort();;
            QMessageBox::information(this, tr("The connection was refused"),
                                     tr("The connection was refused by the peer. "
                                        "Make sure the flashlight server is running, "
                                        "and check that the host name and port "
                                        "settings are correct."));
            break;
        default:
            qDebug() << "The following error occurred:" << _tcpSocket->peerAddress() << _tcpSocket->peerPort()
                     << _tcpSocket->errorString();
            QMessageBox::information(this, tr("Error"),
                                     tr("The following error occurred: %1.")
                                             .arg(_tcpSocket->errorString()));
            break;
    }
    _connectButton->setEnabled(true);
    emit accepted();
    emit connectError(true);
}

void ConnectionDialog::slotReadyRead() {
    QByteArray toPlugin;
    QDataStream in(_tcpSocket);
    in.setByteOrder(QDataStream::BigEndian);
    Header header{0, 0};
    qDebug() << "Ready read";
    for (;;) {
        qDebug() << _tcpSocket->property("receive-data-size").toUInt();
        if (!header.length) {
            if (_tcpSocket->bytesAvailable() < sizeof(Header)) {
                break;
            }
            in >> header.type;
            in >> header.length;
        }
        if (_tcpSocket->bytesAvailable() < header.length) {
            break;
        }

        toPlugin.append(header.type);
        toPlugin.append(header.length);

        if ((in.readRawData(toPlugin.data() + sizeof(Header), header.length) < 0)) {
            qDebug() << "Something wrong with message's body size" << "expect" << header.length;
        } else
                emit readMessage(toPlugin);

        toPlugin.clear();
        header = {0, 0};
    }
}

void ConnectionDialog::slotConnected() {
    emit connectError(false);
    _connectButton->setEnabled(true);
    qDebug() << "Connected to" << _tcpSocket->peerAddress() << _tcpSocket->peerPort();
    accept();            //close dialog, NOT socket
}
