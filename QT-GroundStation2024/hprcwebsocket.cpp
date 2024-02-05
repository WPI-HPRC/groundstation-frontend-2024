#include "hprcwebsocket.h"
#include <iostream>

HPRCWebSocket::HPRCWebSocket(QString hostname, int port): hostname(hostname), port(port)
{
    m_socket = new QWebSocket();


    connect(m_socket, SIGNAL(connected()), this, SLOT(onConnected()));
    connect(m_socket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));

//    connect(m_socket, SIGNAL(errorOccurred(QAbstractSocket::SocketError)), this, SLOT(errorOccurred(QAbstractSocket::SocketError)));
    connect(m_socket, SIGNAL(sslErrors(QList<QSslError>)), this, SLOT(sslErrors(QList<QSslError>)));
//    connect(m_socket, SIGNAL(hostFound()), this, SLOT(hostFound()));
    connect(m_socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(stateChanged(QAbstractSocket::SocketState)));
    connect(m_socket, SIGNAL(alertReceived(QSsl::AlertLevel, QSsl::AlertType, const QString &)), this, SLOT(alertReceived(QSsl::AlertLevel, QSsl::AlertType, const QString &)));
    connect(m_socket, SIGNAL(alertSent(QSsl::AlertLevel, QSsl::AlertType, const QString &)), this, SLOT(alertSent(QSsl::AlertLevel, QSsl::AlertType, const QString &)));
//    connect(m_socket, SIGNAL(encrypted()), this, SLOT(encrypted()));
//    connect(m_socket, SIGNAL(encryptedBytesWritten(qint64)), this, SLOT(encryptedBytesWritten(qint64)));
    connect(m_socket, SIGNAL(handshakeInterruptedOnError(const QSslError &)), this, SLOT(handshakeInterruptedOnError(const QSslError &)));
//    connect(m_socket, SIGNAL(modeChanged(QSslSocket::SslMode)), this, SLOT(modeChanged(QSslSocket::SslMode)));
//    connect(m_socket, SIGNAL(newSessionTicketReceived()), this, SLOT(newSessionTicketReceived()));
    connect(m_socket, SIGNAL(peerVerifyError(const QSslError &)), this, SLOT(peerVerifyError(const QSslError &)));
    connect(m_socket, SIGNAL(preSharedKeyAuthenticationRequired(QSslPreSharedKeyAuthenticator *)), this, SLOT(preSharedKeyAuthenticationRequired(QSslPreSharedKeyAuthenticator *)));
    connect(m_socket, SIGNAL(proxyAuthenticationRequired(const QNetworkProxy &, QAuthenticator *)), this, SLOT(proxyAuthenticationRequired(const QNetworkProxy &, QAuthenticator *)));
    connect(m_socket, SIGNAL(aboutToClose()), this, SLOT(aboutToClose()));
    connect(m_socket, SIGNAL(bytesWritten(qint64)), this, SLOT(bytesWritten(qint64)));
//    connect(m_socket, SIGNAL(channelBytesWritten(int, qint64)), this, SLOT(channelBytesWritten(int , qint64)));
//    connect(m_socket, SIGNAL(channelReadyRead(int)), this, SLOT(channelReadyRead(int)));
    connect(m_socket, SIGNAL(readChannelFinished()), this, SLOT(readChannelFinished()));
//    connect(m_socket, SIGNAL(readyRead()), this, SLOT(readyRead()));

//    m_socket->setPeerVerifyMode(QSslSocket::VerifyPeer);

    //    connect(m_socket, SIGNAL(), this, SLOT());
}

void HPRCWebSocket::connectToServer()
{
    m_socket->open(QUrl("wss://hprc-test.entflammen.com:8000"));
}

void HPRCWebSocket::messageReceived(QString message)
{
    emit onTextMessageReceived(message);
}


void HPRCWebSocket::onConnected()
{
    std::cout << "Websocket Connected" << std::endl;
    connect(m_socket, SIGNAL(textMessageReceived(QString)), this, SLOT(messageReceived(QString)));
}

void HPRCWebSocket::onDisconnected()
{
    qDebug() << "Disconnected";
    connectToServer();
}

void HPRCWebSocket::errorOccurred(QAbstractSocket::SocketError err)
{
    qDebug() << "Error: " << err;
}

void HPRCWebSocket::sslErrors(QList<QSslError> errs)
{
    qDebug() << "Errors!";
    for(auto& err : errs)
    {
        qDebug() << err;
    }
}

void HPRCWebSocket::stateChanged(QAbstractSocket::SocketState state)
{
    qDebug() << "Socket state changed: " << state;
}

void HPRCWebSocket::alertReceived(QSsl::AlertLevel level, QSsl::AlertType type, const QString &description)
{
    qDebug() << "Alert received: " << description;
}

void HPRCWebSocket::alertSent(QSsl::AlertLevel level, QSsl::AlertType type, const QString &description)
{
    qDebug() << "Alert sent: " << description;
}

void HPRCWebSocket::encrypted()
{
    qDebug() << "Encrypted";
}

void HPRCWebSocket::encryptedBytesWritten(qint64 written)
{
    qDebug() << "Encrypted bytes written: " << written;
}

void HPRCWebSocket::handshakeInterruptedOnError(const QSslError &error)
{
    qDebug() << "Handshake interrupted on erorr" << error;
}

void HPRCWebSocket::modeChanged(QSslSocket::SslMode mode)
{
    qDebug() << "Ssl Mode changed to: " << mode;
}

void HPRCWebSocket::newSessionTicketReceived()
{
    qDebug() << "New session ticket received";
}

void HPRCWebSocket::peerVerifyError(const QSslError &error)
{
    qDebug() << "Perr verify error: " << error;
}

void HPRCWebSocket::preSharedKeyAuthenticationRequired(QSslPreSharedKeyAuthenticator *authenticator)
{
    qDebug() << "Pre shared key authentication required: " << authenticator;
}

void HPRCWebSocket::hostFound()
{
    qDebug() << "Host found";
}

void HPRCWebSocket::proxyAuthenticationRequired(const QNetworkProxy &proxy, QAuthenticator *authenticator)
{
    qDebug() << "Proxy authentication required: " << proxy << "\nAuthenticator: " << authenticator;
}

void HPRCWebSocket::aboutToClose()
{
    qDebug() << "About to close";
}

void HPRCWebSocket::bytesWritten(qint64 bytes)
{
    qDebug() << "Bytes written: " << bytes;
}

void HPRCWebSocket::channelBytesWritten(int channel, qint64 bytes)
{
    qDebug() << "Bytes written: " << bytes << " on channel: " << channel;
}

void HPRCWebSocket::channelReadyRead(int channel)
{
    qDebug() << "Channel ready read: " << channel;
}

void HPRCWebSocket::readChannelFinished()
{
    qDebug() << "Read channel finished";
}

void HPRCWebSocket::readyRead()
{
    qDebug() << "Ready read";
}
