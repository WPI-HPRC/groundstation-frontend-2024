#ifndef HPRCGRAPHICSWIDGET_H
#define HPRCGRAPHICSWIDGET_H

#include "hprcdisplaywidget.h"
#include <QGraphicsView>

class hprcGraphicsWidget : public hprcDisplayWidget
{
public:
    explicit hprcGraphicsWidget(QWidget *parent = nullptr, bool mouseTracking = false);

    QGraphicsView* graphicsView;
    QGraphicsScene* graphicsScene;

    void mouseMoveEvent(QMouseEvent *e);
};

#endif // HPRCGRAPHICSWIDGET_H
