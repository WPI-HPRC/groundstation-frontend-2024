//
// Created by William Scheirey on 4/30/24.
//

#include "RocketConnectionIndicator.h"
#include <QSizePolicy>
#include <QApplication>

RocketConnectionIndicator::RocketConnectionIndicator(QWidget *parent): ConnectionIndicator(parent)
{
    label = "Rocket";

    foreach (QWidget *w, qApp->topLevelWidgets()) {
            if (MainWindow *mainWin = qobject_cast<MainWindow *>(w)) {
                connect(mainWin, &MainWindow::rocketPacketReceived, this, &RocketConnectionIndicator::packetReceived);
            }
        }

    connect(&updateTimer, &QTimer::timeout, this, &RocketConnectionIndicator::tick);
    updateTimer.start(5000);
}

void RocketConnectionIndicator::tick()
{
    if(QDateTime::currentSecsSinceEpoch() - this->lastPacketTime > 5 && this->state != RED)
    {
        this->state = RED;
        this->repaint();
    }
}

void RocketConnectionIndicator::packetReceived()
{
    this->lastPacketTime = QDateTime::currentSecsSinceEpoch();
    if(this->state != GREEN)
    {
        this->state = GREEN;
        this->repaint();
    }
}