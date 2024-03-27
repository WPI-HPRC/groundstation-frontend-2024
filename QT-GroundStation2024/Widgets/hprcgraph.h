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

    void drawDetailedView(QPainter* p, DrawResources* drawResources, MainWindow::dataPoint* m_latest) override;
};

#endif // HPRCGRAPH_H
