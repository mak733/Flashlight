/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtWidgets>
#include <QtNetwork>

#include "connectiondialog.h"
#include "stream_interface.h"

//! [0]
ConnectionDialog::ConnectionDialog(QWidget *parent)
    : QDialog(parent)
    , _hostLineEdit(new QLineEdit)
    , _portLineEdit(new QLineEdit)
    , _connectButton(new QPushButton(tr("Connect")))
    , _loadButton(new QPushButton(tr("Load")))
    , _saveButton(new QPushButton(tr("Save")))
    , m_tcpSocket(new QTcpSocket(this))
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    QString ipRange = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
    _ipRegex =  QRegExp("^" + ipRange
                     + "\\." + ipRange
                     + "\\." + ipRange
                     + "\\." + ipRange + "$");

    QRegExpValidator *ipValidator = new QRegExpValidator(_ipRegex, this);
    //_hostnameRegex = QRegExp("^\\S+$");
    PluginManager *plugin = PluginManager::instance();
    CoreInterface *core = plugin->getPlugin(Protocol::TLV, Command::On);
    //qDebug() << core->name();
    _hostLineEdit->setValidator(ipValidator);
    _hostLineEdit->setText("127.0.0.1");

    _portLineEdit->setValidator(new QIntValidator(1, 65535, this));
    _portLineEdit->setText("9999");

    auto hostLabel = new QLabel(tr("&Server name:"));
    hostLabel->setBuddy(_hostLineEdit);
    auto portLabel = new QLabel(tr("&Server port:"));
    portLabel->setBuddy(_portLineEdit);

    _statusLabel = new QLabel(tr("This examples requires that you run the "
                                "Fortune Server example as well."));

    _connectButton->setDefault(true);

    auto quitButton = new QPushButton(tr("Quit"));

    auto buttonBox = new QDialogButtonBox;
    buttonBox->addButton(_loadButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(quitButton, QDialogButtonBox::RejectRole);
    buttonBox->addButton(_connectButton, QDialogButtonBox::AcceptRole);
    buttonBox->addButton(_saveButton, QDialogButtonBox::ActionRole);

    in.setDevice(m_tcpSocket);
    in.setVersion(QDataStream::Qt_4_0);


    connect(_connectButton, &QAbstractButton::clicked,
            this, &ConnectionDialog::requestNewConnection);
    connect(quitButton, &QAbstractButton::clicked, this, &QWidget::close);


    //connect(tcpSocket, &QIODevice::readyRead, this, &ConnectionSettingDialog::readFortune);
    connect(m_tcpSocket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),
            this, &ConnectionDialog::displayError);
    connect(m_tcpSocket, SIGNAL(connected()), SLOT(slotConnected()));
    connect(m_tcpSocket, SIGNAL(readyRead()), SLOT(slotReadyRead()));


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

}

ConnectionDialog::~ConnectionDialog()
{
    qDebug() << "~ConnectionDialog()";
}


void ConnectionDialog::requestNewConnection()
{
    m_tcpSocket->abort();
    m_tcpSocket->connectToHost(_hostLineEdit->text(),
                             _portLineEdit->text().toInt());

}

void ConnectionDialog::readFortune()
{
    in.startTransaction();

    QString nextFortune;
    in >> nextFortune;

    if (!in.commitTransaction())
        return;

    if (nextFortune == currentFortune) {
        QTimer::singleShot(0, this, &ConnectionDialog::requestNewConnection);
        return;
    }

    currentFortune = nextFortune;
    _statusLabel->setText(currentFortune);
    _connectButton->setEnabled(true);
}

void ConnectionDialog::displayError(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        QMessageBox::information(this, tr("Fortune Client"),
                                 tr("The host closed the connection."));
        break;
    case QAbstractSocket::HostNotFoundError:
        QMessageBox::information(this, tr("Fortune Client"),
                                 tr("The host was not found. Please check the "
                                    "host name and port settings."));
        break;
    case QAbstractSocket::ConnectionRefusedError:
        QMessageBox::information(this, tr("Fortune Client"),
                                 tr("The connection was refused by the peer. "
                                    "Make sure the fortune server is running, "
                                    "and check that the host name and port "
                                    "settings are correct."));
        break;
    default:
        QMessageBox::information(this, tr("Fortune Client"),
                                 tr("The following error occurred: %1.")
                                 .arg(m_tcpSocket->errorString()));
    }
    emit accepted();
    emit connectError(true);
    _connectButton->setEnabled(true);
}

void ConnectionDialog::slotReadyRead()
{
    QDataStream in(m_tcpSocket);
    in.setVersion(QDataStream::Qt_4_2);
    PluginManager *plugin = PluginManager::instance();
    //_plugin = qobject_cast<StreamInterface *>(plugin->streamPlugin());
    for (;;) {
        if (!m_nNextBlockSize) {
            if (m_tcpSocket->bytesAvailable() < sizeof(quint16)) {
                break;
            }
            in >> m_nNextBlockSize;
        }

        if (m_tcpSocket->bytesAvailable() < m_nNextBlockSize) {
            break;
        }
        QTime   time;
        QString str;
        in >> time >> str;
        m_nNextBlockSize = 0;
    }
}

void ConnectionDialog::slotConnected()
{
    qDebug() << "connected!";
    close();
}





