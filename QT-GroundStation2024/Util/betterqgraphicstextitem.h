#ifndef BETTERQGRAPHICSTEXTITEM_H
#define BETTERQGRAPHICSTEXTITEM_H

#include <QGraphicsTextItem>
#include <QFont>
#include <QPainter>

class BetterQGraphicsTextItem: public QGraphicsTextItem {
public:
    QRectF geometry;
    QFlags<Qt::Alignment::enum_type> alignment;

    explicit BetterQGraphicsTextItem(QRectF geometry, QFlags<Qt::Alignment::enum_type> alignment, QString text, QGraphicsItem *parent=0) :
        QGraphicsTextItem(parent), geometry(geometry), alignment(alignment)
    {
        opacity = 1;
        this->setPlainText(text);
    }
    virtual ~BetterQGraphicsTextItem() {}
    QRectF boundingRect() const { return geometry; }


    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
    {
        if(opacity)
            painter->setOpacity(opacity);

        painter->setFont(font);

        painter->setBrush(QBrush(color));
        painter->setPen(QPen(color));

        painter->drawText(geometry, this->toPlainText(), alignment);
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
    float opacity;
    QFont font;
    QColor color;
};

#endif // BETTERQGRAPHICSTEXTITEM_H
