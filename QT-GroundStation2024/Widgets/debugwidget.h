#ifndef DEBUGWIDGET_H
#define DEBUGWIDGET_H

#include "hprcgraphicswidget.h"
#include <Util/betterqgraphicstextitem.h>

class debugWidget : public hprcGraphicsWidget
{   
public:
    debugWidget(QWidget *parent = nullptr);
    QWidget *parent;

    QMap<QString, BetterQGraphicsTextItem *> *textItems;
    QList<QString> dataNames;
    void updateSize();

    void drawData(QString dataName, QString dataString, int xIndex, int yIndex, QRectF drawBox, float itemSpacing, float textItemWidth, float textItemHeight);
};

#endif // DEBUGWIDGET_H
