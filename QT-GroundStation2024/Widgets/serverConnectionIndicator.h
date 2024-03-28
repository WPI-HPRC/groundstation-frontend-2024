//
// Created by William Scheirey on 2/23/24.
//

#ifndef QT_GROUNDSTATION2024_SERVERCONNECTIONINDICATOR_H
#define QT_GROUNDSTATION2024_SERVERCONNECTIONINDICATOR_H


#include "hprcgraphicswidget.h"
#include <Util/betterqgraphicstextitem.h>
#include <qabstractsocket.h>

class serverConnectionIndicator : public hprcGraphicsWidget
{
Q_OBJECT
public:
    explicit serverConnectionIndicator(QWidget *parent = nullptr);

    QGraphicsEllipseItem *indicator;
    BetterQGraphicsTextItem *connectionLabel;
    QAbstractSocket::SocketState socketState;

public slots:
    void socketStateChanged(QAbstractSocket::SocketState state);
};


#endif //QT_GROUNDSTATION2024_SERVERCONNECTIONINDICATOR_H
