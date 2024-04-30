//
// Created by William Scheirey on 4/30/24.
//

#ifndef QT_GROUNDSTATION2024_SERVERCONNECTIONINDICATOR_H
#define QT_GROUNDSTATION2024_SERVERCONNECTIONINDICATOR_H
#include "ConnectionIndicator.h"


class ServerConnectionIndicator: public ConnectionIndicator
{
    Q_OBJECT
public:
    ServerConnectionIndicator(QWidget *parent = nullptr);

public slots:
    void socketStateChanged(QAbstractSocket::SocketState socketState);
};


#endif //QT_GROUNDSTATION2024_SERVERCONNECTIONINDICATOR_H
