#include "hprcwebsocket.h"
#include <iostream>

#define DEBUG_WEBSOCKETS 0

HPRCWebSocket::HPRCWebSocket(QString hostname, int port): hostname(hostname), port(port)
{
    m_socket = new QWebSocket();
    retryConnection = true;


    connect(m_socket, SIGNAL(connected()), this, SLOT(onConnected()));
    connect(m_socket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));

#if DEBUG_WEBSOCKETS
    connect(m_socket, SIGNAL(sslErrors(QList<QSslError>)), this, SLOT(sslErrors(QList<QSslError>)));

    connect(m_socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(stateChanged(QAbstractSocket::SocketState)));

    connect(m_socket, SIGNAL(handshakeInterruptedOnError(const QSslError &)), this, SLOT(handshakeInterruptedOnError(const QSslError &)));

    connect(m_socket, SIGNAL(peerVerifyError(const QSslError &)), this, SLOT(peerVerifyError(const QSslError &)));
    connect(m_socket, SIGNAL(aboutToClose()), this, SLOT(aboutToClose()));
    connect(m_socket, SIGNAL(bytesWritten(qint64)), this, SLOT(bytesWritten(qint64)));
    connect(m_socket, SIGNAL(readChannelFinished()), this, SLOT(readChannelFinished()));
#endif

    connect(m_socket, SIGNAL(textMessageReceived(QString)), this, SLOT(messageReceived(QString)));
    connect(this, SIGNAL(open(QUrl)), m_socket, SLOT(open(QUrl)));
    connect(this, SIGNAL(closeTheSocket(QWebSocketProtocol::CloseCode,QString)), m_socket, SLOT(close(QWebSocketProtocol::CloseCode,QString)));
    connect(this, SIGNAL(ping(QByteArray)), m_socket, SLOT(ping(QByteArray)));
}

void HPRCWebSocket::connectToServer()
{
    moveToThread(&thread);
    connect(&thread, SIGNAL(started()), this, SLOT(_connectToServer()));
    thread.start();
}

void HPRCWebSocket::_connectToServer()
{
#if DEBUG_WEBSOCKETS
    qDebug("Attemtping to connect to server");
#endif
    emit open(QUrl("wss://hprc-test.entflammen.com:8000"));
}

void HPRCWebSocket::_ping()
{
    emit ping(QByteArray());
}

void HPRCWebSocket::close()
{
    retryConnection = false;
    emit closeTheSocket(QWebSocketProtocol::CloseCodeNormal, "Closing time");
}

void HPRCWebSocket::messageReceived(QString message)
{
    emit onTextMessageReceived(message);
}

void HPRCWebSocket::onConnected()
{
#if DEBUG_WEBSOCKETS
    qDebug("Websocket Connected");
#endif
}

void HPRCWebSocket::onDisconnected()
{
#if DEBUG_WEBSOCKETS
    qDebug("Websocket Disconnected");
#endif

    if(retryConnection)
    {
#if DEBUG_WEBSOCKETS
        qDebug("Trying to reconnect");
#endif
        QTimer::singleShot(3000, this, &HPRCWebSocket::_connectToServer);
    }
}

void HPRCWebSocket::sslErrors(QList<QSslError> errs)
{
    qDebug("Errors!");
    for(auto& err : errs)
    {
        qDebug() << err;
    }
}

void HPRCWebSocket::stateChanged(QAbstractSocket::SocketState state)
{
    qDebug() << "Socket state changed: " << state;
}

void HPRCWebSocket::handshakeInterruptedOnError(const QSslError &error)
{
    qDebug() << "Handshake interrupted on erorr" << error;
}

void HPRCWebSocket::peerVerifyError(const QSslError &error)
{
    qDebug() << "Perr verify error: " << error;
}

void HPRCWebSocket::aboutToClose()
{
    qDebug("About to close");
}

void HPRCWebSocket::bytesWritten(qint64 bytes)
{
    qDebug() << "Bytes written: " << bytes;

    if(m_socket->state() == QAbstractSocket::ConnectingState)
    {
        emit ping(QByteArray());
    }
}

void HPRCWebSocket::readChannelFinished()
{
    qDebug("Read channel finished");
}
