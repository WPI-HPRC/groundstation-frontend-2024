#include "hprcgraphicswidget.h"
#include "../Util/mousetrackinggraphicsview.h"
#include <QVBoxLayout>

hprcGraphicsWidget::hprcGraphicsWidget(QWidget *parent, bool mouseTracking): hprcDisplayWidget(parent)
{
//    return;
    graphicsView = mouseTracking ? new MouseTrackingGraphicsView(this) : new QGraphicsView(this);
    graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    graphicsView->setViewportUpdateMode(QGraphicsView::NoViewportUpdate);

    // Set up layout
    QHBoxLayout* layout = new QHBoxLayout(this);
    graphicsView->setSceneRect(layout->geometry());
    layout->setObjectName("GraphicsWidget Layout");
    layout->addWidget(graphicsView);
    this->setLayout(layout);

    graphicsScene = new QGraphicsScene(this);

    graphicsView->setScene(graphicsScene);
}


void hprcGraphicsWidget::mouseMoveEvent(QMouseEvent *e)
{
    m_mousePos = e->pos();
}
