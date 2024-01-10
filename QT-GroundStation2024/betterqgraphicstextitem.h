#ifndef BETTERQGRAPHICSTEXTITEM_H
#define BETTERQGRAPHICSTEXTITEM_H

#include <QGraphicsTextItem>

class BetterQGraphicsTextItem: public QGraphicsTextItem {
public:
    explicit BetterQGraphicsTextItem(QRectF geometry, QFlags<Qt::Alignment::enum_type> alignment, QString text, QGraphicsItem *parent=0) :
        QGraphicsTextItem(parent), geometry(geometry), alignment(alignment), text(text)
    {}
    virtual ~BetterQGraphicsTextItem() {}
    QRectF boundingRect() const { return geometry; }
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
    {
        if(opacity)
            painter->setOpacity(opacity);
//        if(font != nullptr)
        painter->setFont(font);

        painter->setBrush(QBrush(color));
        painter->setPen(QPen(color));

        painter->drawText(geometry, text, alignment);
    }

    void setOpacity(float _opacity)
    {
        opacity = _opacity;
    }

    void setFont(QFont _font)
    {
        font = _font;
    }

    void setDefaultTextColor(QColor c)
    {
        color = c;
    }

private:
    QRectF geometry;
    QString text;
    QFlags<Qt::Alignment::enum_type> alignment;

    float opacity;
    QFont font;
    QColor color;
};

#endif // BETTERQGRAPHICSTEXTITEM_H
