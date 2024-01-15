#include "hprcsubgraph.h"

HPRCSubGraph::HPRCSubGraph(QString labelText, QGraphicsScene* graphicsScene)
{
    this->scene = graphicsScene;

    this->textLabel = new BetterQGraphicsTextItem(QRectF(0, 0, 0, 0), Qt::AlignVCenter, labelText);
    graphicsScene->addItem(this->textLabel);
    this->valueLabel = new BetterQGraphicsTextItem(QRectF(0, 0, 0, 0), Qt::AlignVCenter | Qt::AlignRight, labelText);
    graphicsScene->addItem(this->valueLabel);

    this->bottomLine = new QGraphicsLineItem();
    graphicsScene->addItem(this->bottomLine);

    this->polygon = new QGraphicsPolygonItem();
    graphicsScene->addItem(this->polygon);

    this->maxTick = new QGraphicsLineItem();
    graphicsScene->addItem(this->maxTick);
    this->minTick = new QGraphicsLineItem();
    graphicsScene->addItem(this->minTick);

    this->maxValueLabel = new BetterQGraphicsTextItem(QRectF(0, 0, 0, 0), Qt::AlignVCenter | Qt::AlignRight, "0");
    graphicsScene->addItem(this->maxValueLabel);
    this->minValueLabel = new BetterQGraphicsTextItem(QRectF(0, 0, 0, 0), Qt::AlignVCenter | Qt::AlignRight, "0");
    graphicsScene->addItem(this->minValueLabel);

    this->tooltipRect = new QGraphicsRectItem();
    graphicsScene->addItem(tooltipRect);
    this->tooltipCenterLine = new QGraphicsLineItem();
    graphicsScene->addItem(tooltipCenterLine);
    this->tooltipCircle = new QGraphicsEllipseItem();
    graphicsScene->addItem(tooltipCircle);
    this->tooltipValue = new BetterQGraphicsTextItem(QRectF(0, 0, 0, 0), Qt::AlignVCenter, "0");
    graphicsScene->addItem(tooltipValue);
}

void HPRCSubGraph::showTooltip()
{
    tooltipRect->show();
    tooltipCenterLine->show();
    tooltipCircle->show();
    tooltipValue->show();

    maxValueLabel->hide();
    minValueLabel->hide();
    valueLabel->hide();

    maxTick->hide();
    minTick->hide();
}

void HPRCSubGraph::hideTooltip()
{
    tooltipRect->hide();
    tooltipCenterLine->hide();
    tooltipCircle->hide();
    tooltipValue->hide();

    maxValueLabel->show();
    minValueLabel->show();
    valueLabel->show();

    maxTick->show();
    minTick->show();
}



