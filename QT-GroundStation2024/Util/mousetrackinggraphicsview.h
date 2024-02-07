#ifndef MOUSETRACKINGGRAPHICSVIEW_H
#define MOUSETRACKINGGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QPoint>
#include <QMouseEvent>
#include "../Widgets/hprcdisplaywidget.h"

class MouseTrackingGraphicsView : public QGraphicsView
{
public:

    QPointF mousePos;
    hprcDisplayWidget* widget;

    MouseTrackingGraphicsView(hprcDisplayWidget* parent = nullptr) : QGraphicsView(parent), widget(parent) {
        setMouseTracking(true);
    }

protected:
    void mouseMoveEvent(QMouseEvent* event) override {
        // It's getting shifted for some reason, so shift it back
        widget->m_mousePos = QPoint(event->pos().x(), event->pos().y());
    }

    virtual void wheelEvent(QWheelEvent* event) Q_DECL_OVERRIDE {
        event->accept();
    }
};

#endif // MOUSETRACKINGGRAPHICSVIEW_H
