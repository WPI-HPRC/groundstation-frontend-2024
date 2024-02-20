#ifndef HPRCGPSLOCKINDICATOR_H
#define HPRCGPSLOCKINDICATOR_H

#include "hprcgraphicswidget.h"

class hprcGpsLockIndicator : public hprcGraphicsWidget
{
    Q_OBJECT
public:
   explicit hprcGpsLockIndicator(QWidget *parent = nullptr);

    QGraphicsEllipseItem *lockIndicator;
    BetterQGraphicsTextItem *satellitesLabel;
    QGraphicsRectItem *outline;
};

#endif // HPRCGPSLOCKINDICATOR_H
