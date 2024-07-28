#ifndef P_DEBUGWIDGET_H
#define P_DEBUGWIDGET_H

#include "hprcgraphicswidget.h"

class p_debugWidget : public hprcGraphicsWidget
{
public:
    p_debugWidget(QWidget *parent = nullptr);
    QWidget *parent;

    QMap<QString, BetterQGraphicsTextItem *> *textItems;
    QList<QString> dataNames;
    void updateSize();

    void drawData(QString dataName, QString dataString, int xIndex, int yIndex, QRectF drawBox, float itemSpacing, float textItemWidth, float textItemHeight);
};

#endif // P_DEBUGWIDGET_H
