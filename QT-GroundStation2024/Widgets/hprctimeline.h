#ifndef HPRCTIMELINE_H
#define HPRCTIMELINE_H

#include "hprcgraphicswidget.h"
#include "qapplication.h"
#include "../Util/roundedQGraphicsRectItem.h"

class hprcTimeline : public hprcGraphicsWidget
{
    Q_OBJECT
public:
    RoundedQGraphicsRectItem *timelineSlot;
    RoundedQGraphicsRectItem *timelineFill;
    QList<QGraphicsTextItem> *textItems;

    explicit hprcTimeline(QWidget *parent)
        : hprcGraphicsWidget(parent, false)
    {
        graphicsView->setObjectName("Timeline");

        m_widgetType = HPRC_Timeline;

        this->timelineSlot = new RoundedQGraphicsRectItem();
        this->timelineSlot->setCacheMode(QGraphicsItem::DeviceCoordinateCache);
        this->graphicsScene->addItem(this->timelineSlot);

        this->timelineFill = new RoundedQGraphicsRectItem();
        this->timelineFill->setCacheMode(QGraphicsItem::DeviceCoordinateCache);
        this->graphicsScene->addItem(this->timelineFill);



        foreach (QWidget *w, qApp->topLevelWidgets())
        {
            if (MainWindow* mainWin = qobject_cast<MainWindow*>(w))
            {
                connect(mainWin, SIGNAL(stateUpdated(int)), this, SLOT(updateFilled(int)));
            }
        }

        qDebug("Making timeline!");
    }

private:
//    void setupStateMap()

signals:
};

#endif // HPRCTIMELINE_H
