#ifndef HPRCWEBSOCKET_H
#define HPRCWEBSOCKET_H

#include <QtWebSockets>
#include <qabstractsocket.h>

class HPRCWebSocket : public QObject
{

    Q_OBJECT

public:
    HPRCWebSocket(QString hostname, int port);
    QString hostname;
    int port;

    void connectToServer();
    void close();

private:
    QWebSocket* m_socket;
    QThread thread;
    void _ping();
    bool retryConnection;

public slots:
    void onConnected();
    void onDisconnected();
    void sslErrors(QList<QSslError> errs);
    void stateChanged(QAbstractSocket::SocketState state);
    void handshakeInterruptedOnError(const QSslError &error);
    void peerVerifyError(const QSslError &error);
    void aboutToClose();
    void bytesWritten(qint64 bytes);
    void readChannelFinished();
    void messageReceived(QString message);
    void _connectToServer();


signals:
    void onTextMessageReceived(QString message);
    void open(QUrl);
    void closeTheSocket(QWebSocketProtocol::CloseCode,QString);
    void ping(QByteArray);
};

#endif // HPRCWEBSOCKET_H
