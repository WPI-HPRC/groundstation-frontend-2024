#ifndef ROUNDEDQGRAPHICSRECTITEM_H
#define ROUNDEDQGRAPHICSRECTITEM_H

#include <QGraphicsRectItem>
#include <QPainter>

class RoundedQGraphicsRectItem: public QGraphicsRectItem
{
    qreal xRadius;
    qreal yRadius;
    explicit RoundedQGraphicsRectItem(qreal xRadius, qreal yRadius, QWidget *parent = nullptr): xRadius(xRadius), yRadius(yRadius), QGraphicsRectItem(parent){}

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override
    {
        painter->drawRoundedRect(this->rect(), xRadius, yRadius);
    }
};

#endif // ROUNDEDQGRAPHICSRECTITEM_H
