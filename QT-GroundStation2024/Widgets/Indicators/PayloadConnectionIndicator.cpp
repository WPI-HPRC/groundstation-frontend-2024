//
// Created by William Scheirey on 4/30/24.
//

#include "PayloadConnectionIndicator.h"
#include <QApplication>

PayloadConnectionIndicator::PayloadConnectionIndicator(QWidget *parent): ConnectionIndicator(parent)
{
    label = "Payload";
            foreach (QWidget *w, qApp->topLevelWidgets()) {
            if (MainWindow *mainWin = qobject_cast<MainWindow *>(w)) {
                connect(mainWin, &MainWindow::payloadPacketReceived, this, &PayloadConnectionIndicator::packetReceived);
            }
        }

    connect(&updateTimer, &QTimer::timeout, this, &PayloadConnectionIndicator::tick);
    updateTimer.start(5000);
}

void PayloadConnectionIndicator::tick()
{
    if(QDateTime::currentSecsSinceEpoch() - this->lastPacketTime > 5 && this->state != RED)
    {
        this->state = RED;
        this->repaint();
    }
}

void PayloadConnectionIndicator::packetReceived()
{
    this->lastPacketTime = QDateTime::currentSecsSinceEpoch();
    if(this->state != GREEN)
    {
        this->state = GREEN;
        this->repaint();
    }
}
