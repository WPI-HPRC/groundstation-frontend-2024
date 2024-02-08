#include "hprcgraphicswidget.h"
#include "../Util/mousetrackinggraphicsview.h"
#include <QVBoxLayout>

hprcGraphicsWidget::hprcGraphicsWidget(QWidget *parent, bool mouseTracking): hprcDisplayWidget(parent)
{
    graphicsView = mouseTracking ? new MouseTrackingGraphicsView(this) : new QGraphicsView(this);
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
}


void hprcGraphicsWidget::mouseMoveEvent(QMouseEvent *e)
{
    m_mousePos = e->pos();
}
