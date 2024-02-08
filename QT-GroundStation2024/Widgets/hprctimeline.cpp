#include "hprctimeline.h"
#include "../Util/hprcStateMap.h"

hprcTimeline::hprcTimeline(QWidget *parent): hprcGraphicsWidget(parent, false)
{
    graphicsView->setObjectName("Timeline");

    m_widgetType = HPRC_Timeline;

    this->timelineSlot = new RoundedQGraphicsRectItem();
    this->timelineSlot->setCacheMode(QGraphicsItem::DeviceCoordinateCache);
    this->graphicsScene->addItem(this->timelineSlot);

    this->timelineFill = new RoundedQGraphicsRectItem();
    this->timelineFill->setCacheMode(QGraphicsItem::DeviceCoordinateCache);
    this->graphicsScene->addItem(this->timelineFill);

    this->textItems = new QMap<QString, BetterQGraphicsTextItem *>;

    for(const auto& [position, label] : hprcStateMaps::stateMap)
    {
        BetterQGraphicsTextItem *textItem = new BetterQGraphicsTextItem(QRect(0, 0, 0, 0), Qt::AlignRight | Qt::AlignVCenter, label);
        textItem->setCacheMode(QGraphicsItem::DeviceCoordinateCache);
        this->graphicsScene->addItem(textItem);

        this->textItems->insert(label, textItem);
    }


    foreach (QWidget *w, qApp->topLevelWidgets())
    {
        if (MainWindow* mainWin = qobject_cast<MainWindow*>(w))
        {
            connect(mainWin, SIGNAL(stateUpdated(int)), this, SLOT(updateFilled(int)));
        }
    }
}
