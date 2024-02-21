#ifndef DEBUGWIDGET_H
#define DEBUGWIDGET_H

#include "hprcgraphicswidget.h"

class debugWidget : public hprcGraphicsWidget
{   
public:
    debugWidget(QWidget *parent = nullptr);
    QWidget *parent;

    QMap<QString, BetterQGraphicsTextItem *> *textItems;
    QList<QString> dataNames;
    void updateSize();
};

#endif // DEBUGWIDGET_H
