#ifndef HPRCGPSLOCKINDICATOR_H
#define HPRCGPSLOCKINDICATOR_H

#include "hprcgraphicswidget.h"
#include <Util/betterqgraphicstextitem.h>

class hprcGpsLockIndicator : public hprcGraphicsWidget
{
    Q_OBJECT
public:
    explicit hprcGpsLockIndicator(QWidget *parent = nullptr);

    QGraphicsEllipseItem *lockIndicator;
    BetterQGraphicsTextItem *lockLabel;
    BetterQGraphicsTextItem *satellitesLabel;
};

#endif // HPRCGPSLOCKINDICATOR_H
