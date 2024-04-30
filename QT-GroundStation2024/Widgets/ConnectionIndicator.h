//
// Created by William Scheirey on 2/23/24.
//

#ifndef QT_GROUNDSTATION2024_CONNECTIONINDICATOR_H
#define QT_GROUNDSTATION2024_CONNECTIONINDICATOR_H


#include "hprcgraphicswidget.h"

class ConnectionIndicator : public hprcGraphicsWidget
{
Q_OBJECT
public:
    explicit ConnectionIndicator(QWidget *parent = nullptr);

    QGraphicsEllipseItem *indicator;
    BetterQGraphicsTextItem *connectionLabel;
    QString label;
    QGraphicsRectItem *bg;

    enum ConnectionState
    {
        RED,
        YELLOW,
        GREEN
    };

    ConnectionState state;
};


#endif //QT_GROUNDSTATION2024_CONNECTIONINDICATOR_H
