#ifndef HPRCGRAPH_H
#define HPRCGRAPH_H

#include "hprcdisplaywidget.h"
#include "hprcsubgraph.h"
#include "./Util/mousetrackinggraphicsview.h"
#include "qapplication.h"
#include <QVBoxLayout>

class hprcGraph : public hprcDisplayWidget
{
public:
    QGraphicsView* graphicsView;
    QGraphicsScene* graphicsScene;

    QGraphicsRectItem* bgRect;
    QGraphicsRectItem* outlineRect;

    HPRCSubGraph* altSubGraph;
    HPRCSubGraph* velSubGraph;
    HPRCSubGraph* accelSubGraph;

    explicit hprcGraph(QWidget *parent = nullptr) :
        hprcDisplayWidget(parent)
    {
        graphicsView = new MouseTrackingGraphicsView(this);
        graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        graphicsView->setViewportUpdateMode(QGraphicsView::NoViewportUpdate);

        // Set up layout
        QVBoxLayout* layout = new QVBoxLayout(this);
        layout->addWidget(graphicsView);
        this->setLayout(layout);

        graphicsView->setObjectName("Graphs");

        graphicsScene = new QGraphicsScene(this);
        graphicsView->setScene(graphicsScene);

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

    void mouseMoveEvent(QMouseEvent *e)
    {
        m_mousePos = e->pos();
    }

};

#endif // HPRCGRAPH_H
