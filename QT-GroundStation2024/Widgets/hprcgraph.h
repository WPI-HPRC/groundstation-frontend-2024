#ifndef HPRCGRAPH_H
#define HPRCGRAPH_H

#include "hprcgraphicswidget.h"
#include "hprcsubgraph.h"
#include "./Util/mousetrackinggraphicsview.h"
#include "qapplication.h"
#include <QVBoxLayout>

class hprcGraph : public hprcGraphicsWidget
{
public:
    QGraphicsRectItem* bgRect;
    QGraphicsRectItem* outlineRect;

    HPRCSubGraph* altSubGraph;
    HPRCSubGraph* velSubGraph;
    HPRCSubGraph* accelSubGraph;

    explicit hprcGraph(QWidget *parent = nullptr);
};

#endif // HPRCGRAPH_H
