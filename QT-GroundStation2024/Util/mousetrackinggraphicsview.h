#ifndef MOUSETRACKINGGRAPHICSVIEW_H
#define MOUSETRACKINGGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QPoint>
#include <Widgets/hprcdisplaywidget.h>

class MouseTrackingGraphicsView : public QGraphicsView
{
public:

    QPointF mousePos;
    hprcDisplayWidget* graph;

    MouseTrackingGraphicsView(hprcGraph* parent = nullptr) : QGraphicsView(parent), graph(parent) {
        setMouseTracking(true);
    }

protected:
    void mouseMoveEvent(QMouseEvent* event) override {
        // It's getting shifted for some reason, so shift it back
        graph->m_mousePos = QPoint(event->pos().x(), event->pos().y());
    }

    virtual void wheelEvent(QWheelEvent* event) Q_DECL_OVERRIDE {
        event->accept();
    }
};

#endif // MOUSETRACKINGGRAPHICSVIEW_H
