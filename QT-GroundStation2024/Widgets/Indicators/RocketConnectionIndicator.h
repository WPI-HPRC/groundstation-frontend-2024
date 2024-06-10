//
// Created by William Scheirey on 4/30/24.
//

#ifndef QT_GROUNDSTATION2024_ROCKETCONNECTIONINDICATOR_H
#define QT_GROUNDSTATION2024_ROCKETCONNECTIONINDICATOR_H

#include "connectionIndicator.h"

class RocketConnectionIndicator: public ConnectionIndicator
{
public:
    RocketConnectionIndicator(QWidget *parent = nullptr);
    unsigned long long lastPacketTime;

public slots:
    void packetReceived();
    void tick();

private:
    QTimer updateTimer;
};


#endif //QT_GROUNDSTATION2024_ROCKETCONNECTIONINDICATOR_H
