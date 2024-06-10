//
// Created by William Scheirey on 4/30/24.
//

#ifndef QT_GROUNDSTATION2024_PAYLOADCONNECTIONINDICATOR_H
#define QT_GROUNDSTATION2024_PAYLOADCONNECTIONINDICATOR_H
#include "connectionIndicator.h"

class PayloadConnectionIndicator: public ConnectionIndicator
{
public:
    PayloadConnectionIndicator(QWidget *parent = nullptr);
    unsigned long long lastPacketTime;

public slots:
    void packetReceived();
    void tick();

private:
    QTimer updateTimer;
};


#endif //QT_GROUNDSTATION2024_PAYLOADCONNECTIONINDICATOR_H
