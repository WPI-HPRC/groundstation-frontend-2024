#ifndef HPRCSUBGRAPH_H
#define HPRCSUBGRAPH_H

#include "Util/betterqgraphicstextitem.h"
#include <QGraphicsView>

class HPRCSubGraph
{
public:

    explicit HPRCSubGraph(QString labelText, QGraphicsScene* graphicsScene);

    void showTooltip();
    void hideTooltip();

    QGraphicsScene* scene;

    BetterQGraphicsTextItem* textLabel;
    BetterQGraphicsTextItem* valueLabel;

    QGraphicsLineItem* bottomLine;

    QGraphicsPolygonItem* polygon;

    QGraphicsLineItem* maxTick;
    QGraphicsLineItem* minTick;

    BetterQGraphicsTextItem* maxValueLabel;
    BetterQGraphicsTextItem* minValueLabel;

    QGraphicsRectItem* tooltipRect;
    QGraphicsLineItem* tooltipCenterLine;
    QGraphicsEllipseItem* tooltipCircle;
    BetterQGraphicsTextItem* tooltipValue;
};

#endif // HPRCSUBGRAPH_H
