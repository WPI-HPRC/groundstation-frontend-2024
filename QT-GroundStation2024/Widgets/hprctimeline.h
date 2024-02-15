#ifndef HPRCTIMELINE_H
#define HPRCTIMELINE_H

#include "hprcgraphicswidget.h"
#include "qapplication.h"
#include "../Util/roundedQGraphicsRectItem.h"
#include "../Util/betterqgraphicstextitem.h"

class hprcTimeline : public hprcGraphicsWidget
{
    Q_OBJECT
public:
    RoundedQGraphicsRectItem *timelineSlot;
    RoundedQGraphicsRectItem *timelineFill;
    QMap<QString, BetterQGraphicsTextItem *> *textItems;

    explicit hprcTimeline(QWidget *parent = nullptr);

private:

signals:
};

#endif // HPRCTIMELINE_H
