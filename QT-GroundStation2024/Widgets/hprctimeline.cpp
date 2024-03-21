#include "hprctimeline.h"
#include "../Util/hprcStateMap.h"

hprcTimeline::hprcTimeline(QWidget *parent) : hprcGraphicsWidget(parent, false)
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
        QString actualLabel = label;
        BetterQGraphicsTextItem *textItem = new BetterQGraphicsTextItem(QRect(0, 0, 0, 0), Qt::AlignRight | Qt::AlignVCenter, actualLabel.startsWith("-") ? "-" : actualLabel);
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

void HPRCStyle::drawHPRCTimeline(QPainter *p, const hprcTimeline *w)
{

    if(w->rect().width() < 100)
        return;

    // <------------------- math --------------------->//

    QPen bgPen(m_backgroundBrush, 15);

    QPen fgPen(m_highlightBrush, 16);

    QPen tickPen(m_textBrush, 20);

    double aspect = 4; // height / width

    QRect widgetBox = w->rect();

    double drawWidth = widgetBox.width();
    double drawHeight = widgetBox.height();

    // wanted aspect ratio is 1:3
    if(drawHeight / drawWidth < aspect)
    {
        drawWidth = drawHeight/aspect;
    } else {
        drawHeight = drawWidth*aspect;
    }

    drawHeight *= 0.85;
    drawWidth  *= 0.85;

    m_widgetLarge.setPointSize(drawHeight/30);
    m_widgetMedium.setPointSize(drawHeight/30 * 0.75);

    fgPen.setWidth(drawHeight/40 - 5);
    bgPen.setWidth(drawHeight/40);

    double drawX = (widgetBox.width() - drawWidth) / 2.0;
    double drawY = (widgetBox.height() - drawHeight) / 2.0;

    double scaleF = 0.03;

    QRect drawBox(drawX, drawY, drawWidth, drawHeight);
    w->graphicsView->setSceneRect(w->layout()->geometry());

    QPoint bottomRight(drawX + (0.9 - scaleF) * drawWidth, drawBox.bottom());
    QPoint topLeftSlot(drawX + (0.9 - scaleF) * drawWidth, drawY);

    float percent = 0;
    if(w->m_filledPercent == 3) {
        percent = 0.15;
    } else if(w->m_filledPercent == 4 || w->m_filledPercent == 5) {
        percent = 0.5;
    } else if(w->m_filledPercent == 6 || w->m_filledPercent == 7) {
        percent = 0.75;
    } else if(w->m_filledPercent > 7) {
        percent = 1;
    }
    QPoint topLeftFill(drawX + (0.9 - scaleF) * drawWidth, drawY + drawHeight - (drawHeight * (percent)));

    w->graphicsScene->setBackgroundBrush(m_transparentBrush);

    w->timelineSlot->setRect(QRect(topLeftSlot, bottomRight));
    w->timelineSlot->setPen(bgPen);
    w->timelineSlot->setRadii(scaleF*drawWidth/2, scaleF*drawWidth/2);

    w->timelineFill->setRect(QRect(topLeftFill, bottomRight));
    w->timelineFill->setPen(fgPen);
    w->timelineFill->setRadii(scaleF*drawWidth/2, scaleF*drawWidth/2);

    if(w->m_filledPercent > 1)
    {
        w->timelineFill->show();
    }
    else
    {
        w->timelineFill->hide();
    }
    // <------------------- draw --------------------->//

    int tickIndex = 0;
    for(const auto& [position, label] : hprcStateMaps::stateMap)
    {
        BetterQGraphicsTextItem* textItem = (*w->textItems)[label];
        textItem->setFont(m_widgetLarge);
        textItem->setDefaultTextColor(tickPen.color());
        float ypos = position * drawHeight + drawY + p->font().pointSize() * 0.3;
        textItem->geometry = QRectF(QPointF(w->geometry().left(), ypos - 100), QPointF(topLeftSlot.x() - drawWidth/5, ypos + 100));


        if(label.startsWith("-"))
        {
            if(!m_latest->timelineActivated[tickIndex]) {
                textItem->setDefaultTextColor(bgPen.color());
            }
            else {

                textItem->setDefaultTextColor(tickPen.color());
            }
            textItem->setFont(m_widgetMedium);
            QString theLabel = m_latest->timelineTimes[tickIndex];
            textItem->setPlainText(theLabel.append(" -"));
            textItem->geometry = QRectF(QPointF(w->geometry().left(), ypos - 100), QPointF(topLeftSlot.x() - 10, ypos + 100));
            tickIndex++;
        }
    }

    w->graphicsView->viewport()->update();
}
