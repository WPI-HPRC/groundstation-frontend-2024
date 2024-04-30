//
// Created by William Scheirey on 4/30/24.
//

#include "ServerConnectionIndicator.h"
#include <QApplication>

ServerConnectionIndicator::ServerConnectionIndicator(QWidget *parent): ConnectionIndicator(parent)
{
    label = "Server";
    m_widgetType = HPRC_serverConnection;

    m_widgetType = hprcDisplayWidget::HPRC_serverConnection;

    foreach (QWidget *w, qApp->topLevelWidgets()) {
            if (MainWindow *mainWin = qobject_cast<MainWindow *>(w)) {
                connect(mainWin, SIGNAL(webSocketStateChanged(QAbstractSocket::SocketState)), this,
                        SLOT(socketStateChanged(QAbstractSocket::SocketState)));
            }
        }
}

void ServerConnectionIndicator::socketStateChanged(QAbstractSocket::SocketState socketState) {
    switch (socketState)
    {
        case QAbstractSocket::ConnectedState:
            this->state = GREEN;
            break;
        case QAbstractSocket::ConnectingState:
            this->state = YELLOW;
            break;
        default:
            this->state = RED;
            break;
    }
    this->repaint();
}
