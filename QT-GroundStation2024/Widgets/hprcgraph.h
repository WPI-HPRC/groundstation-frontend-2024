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

    explicit hprcGraph(QWidget *parent = nullptr) :
        hprcGraphicsWidget(parent, true)
    {

        graphicsView->setObjectName("Graphs");

        this->altSubGraph = new HPRCSubGraph("Alt (m)", graphicsScene);
        this->velSubGraph = new HPRCSubGraph("VEL (m/s)", graphicsScene);
        this->accelSubGraph = new HPRCSubGraph("ACCEL (m/s²)", graphicsScene);

        bgRect = new QGraphicsRectItem();
        outlineRect = new QGraphicsRectItem();

        graphicsScene->addItem(bgRect);
        graphicsScene->addItem(outlineRect);

        setMouseTracking(true);
        m_widgetType = HPRC_Graph;
        foreach (QWidget *w, qApp->topLevelWidgets())
            if (MainWindow* mainWin = qobject_cast<MainWindow*>(w))
            {
                connect(mainWin, SIGNAL(tick()), this, SLOT(repaint()));
            }
    }
};

#endif // HPRCGRAPH_H
