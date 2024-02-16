#ifndef ROUNDEDQGRAPHICSRECTITEM_H
#define ROUNDEDQGRAPHICSRECTITEM_H

#include <QGraphicsRectItem>
#include <QPainter>

class RoundedQGraphicsRectItem: public QGraphicsRectItem
{
public:
    qreal xRadius = 0;
    qreal yRadius = 0;
    explicit RoundedQGraphicsRectItem(QWidget *parent = nullptr): QGraphicsRectItem(){}

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override
    {
        painter->setPen(this->pen());
        painter->setBrush(this->brush());
        painter->setOpacity(this->opacity());
        painter->drawRoundedRect(this->rect(), xRadius, yRadius);
    }

    void setRadii(qreal x, qreal y)
    {
        this->xRadius = x;
        this->yRadius = y;
    }
};

#endif // ROUNDEDQGRAPHICSRECTITEM_H
