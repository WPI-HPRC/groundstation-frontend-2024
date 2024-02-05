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

private:
    QWebSocket* m_socket;

public slots:
    void onConnected();
    void onDisconnected();
    void readyRead();
    void errorOccurred(QAbstractSocket::SocketError err);
    void sslErrors(QList<QSslError> errs);
    void stateChanged(QAbstractSocket::SocketState state);
    void alertReceived(QSsl::AlertLevel level, QSsl::AlertType type, const QString &description);
    void alertSent(QSsl::AlertLevel level, QSsl::AlertType type, const QString &description);
    void encrypted();
    void encryptedBytesWritten(qint64 written);
    void handshakeInterruptedOnError(const QSslError &error);
    void modeChanged(QSslSocket::SslMode mode);
    void newSessionTicketReceived();
    void peerVerifyError(const QSslError &error);
    void preSharedKeyAuthenticationRequired(QSslPreSharedKeyAuthenticator *authenticator);
    void hostFound();
    void proxyAuthenticationRequired(const QNetworkProxy &proxy, QAuthenticator *authenticator);
    void aboutToClose();
    void bytesWritten(qint64 bytes);
    void channelBytesWritten(int channel, qint64 bytes);
    void channelReadyRead(int channel);
    void readChannelFinished();
    void messageReceived(QString message);

signals:
    void onTextMessageReceived(QString message);

};

#endif // HPRCWEBSOCKET_H
