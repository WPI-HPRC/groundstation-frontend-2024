#include "hprcsubgraph.h"
#include "../stylepainters.h"

HPRCSubGraph::HPRCSubGraph(QString labelText, QGraphicsScene *graphicsScene) {
    this->scene = graphicsScene;

    this->textLabel = new BetterQGraphicsTextItem(QRectF(0, 0, 0, 0), Qt::AlignVCenter, labelText);
    graphicsScene->addItem(this->textLabel);
    this->textLabel->setCacheMode(QGraphicsItem::DeviceCoordinateCache);
    this->valueLabel = new BetterQGraphicsTextItem(QRectF(0, 0, 0, 0), Qt::AlignVCenter | Qt::AlignRight, labelText);
    graphicsScene->addItem(this->valueLabel);
//    this->valueLabel->setCacheMode(QGraphicsItem::ItemCoordinateCache);

    this->bottomLine = new QGraphicsLineItem();
    graphicsScene->addItem(this->bottomLine);
    this->bottomLine->setCacheMode(QGraphicsItem::DeviceCoordinateCache);


    this->polygon = new QGraphicsPolygonItem();
    graphicsScene->addItem(this->polygon);

    this->maxTick = new QGraphicsLineItem();
    graphicsScene->addItem(this->maxTick);
    this->maxTick->setCacheMode(QGraphicsItem::DeviceCoordinateCache);

    this->minTick = new QGraphicsLineItem();
    graphicsScene->addItem(this->minTick);
    this->minTick->setCacheMode(QGraphicsItem::DeviceCoordinateCache);

    this->maxValueLabel = new BetterQGraphicsTextItem(QRectF(0, 0, 0, 0), Qt::AlignVCenter | Qt::AlignRight, "0");
    graphicsScene->addItem(this->maxValueLabel);
    this->minValueLabel = new BetterQGraphicsTextItem(QRectF(0, 0, 0, 0), Qt::AlignVCenter | Qt::AlignRight, "0");
    graphicsScene->addItem(this->minValueLabel);

    this->tooltipRect = new QGraphicsRectItem();
    graphicsScene->addItem(tooltipRect);
    this->tooltipRect->setCacheMode(QGraphicsItem::ItemCoordinateCache);

    this->tooltipCenterLine = new QGraphicsLineItem();
    graphicsScene->addItem(tooltipCenterLine);
    this->tooltipCenterLine->setCacheMode(QGraphicsItem::ItemCoordinateCache);

    this->tooltipCircle = new QGraphicsEllipseItem();
    graphicsScene->addItem(tooltipCircle);
    this->tooltipCircle->setCacheMode(QGraphicsItem::ItemCoordinateCache);

    this->tooltipValue = new BetterQGraphicsTextItem(QRectF(0, 0, 0, 0), Qt::AlignVCenter, "0");
    graphicsScene->addItem(tooltipValue);
}

void HPRCSubGraph::showTooltip() {
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

void HPRCSubGraph::hideTooltip() {
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


void HPRCStyle::drawHPRCSubGraph(QPainter *p, QRectF rect, QColor bg, GraphPointCircularBuffer *data, GraphType graphType,
                            double range, double start, hprcGraph *w, QGraphicsScene *scene, bool drawTooltip) {
    if (rect.height() < 10)
        return;

    HPRCSubGraph *graph;

    switch (graphType) {
        case GRAPH_Altitude:
            graph = w->altSubGraph;
            break;
        case GRAPH_Velocity:
            graph = w->velSubGraph;
            break;
        case GRAPH_Acceleration:
            graph = w->accelSubGraph;
            break;
        default:
            break;
    }

    QBrush lightHighlighterBrush(QColor(255, 255, 255, 255));

    QPointF bottomPt = rect.bottomLeft();
    QLinearGradient gradient(bottomPt, rect.topLeft());

    bg.setAlphaF(0.5);
    gradient.setColorAt(0, bg);
    gradient.setColorAt(1, m_transparentBrush.color());
    bg.setAlphaF(1);

    // Line at the bottom, subtract a pixel from each side to fit nicer
    graph->bottomLine->setLine(QLine(rect.left() + 1, rect.bottom(), rect.right() - 1, rect.bottom()));
    graph->bottomLine->setPen(QPen(bg, 2));
    graph->bottomLine->setZValue(0);;

    double maxValue = graphPointCircularBufferGetMaxValue(data);
    double minValue = graphPointCircularBufferGetMinValue(data);

    // These values are altered sometimes for graph scaling purposes. We need to keep the real max/min for later
    double scaleMax = maxValue;
    double scaleMin = minValue;

    maxValue = fmax(maxValue, 0);
    minValue = fmin(minValue, 0);

    QList<QPoint> pointsToDraw;

    scaleMax = fmaxf(scaleMax, 0);

    double scale = fmax(1.0, scaleMax - scaleMin);

    // Out of sight
    QRectF ptHighlight(-100, 0, 0, 0);
    QPointF highlighted(-100, 0);
    QString ptLabel("");

    bool drawMaxMarker = true;

    double centerY = 0;

    // Setting the centerpoint of the axis
    if (scaleMin < 0) {
        // Move the centerline up away from the bottom of the rectangle so negative points fit on the graph
        centerY = fabs(scaleMin / scale) * rect.height();
        scaleMin = 0;
    } else {
        scale = scaleMax;
    }

    double valYNormalized = 0;
    double yMultiplier = 0;

    double valX = 0;
    double valY = 0;

    double centerLine = rect.top() + rect.height() - centerY;

    double closestDist = std::numeric_limits<double>::max();

    if (abs(scale) < 1) {
        if (scale < -1) {
            scale = -1;
        } else {
            scale = 1;
        }
    }
    for (int i = 0; i < CIRCULAR_BUFFER_LEN; i++) {
        GraphPoint *g = graphPointCircularBufferGetValueAtIndex(data, i);

        if (g->time == 0)
            continue;

        valX = rect.left() + 1 + (g->time - start) / range * (rect.width() - 1); // This was Ben's math. It works

        valYNormalized = g->value / scale;

        // Multiply the normalized value by the distance between the center line and the top or the bottom
        // depending on whether the value is positive or negative. Use some boolean algebra to avoid an if statement
        yMultiplier = ((g->value > 0) * (rect.height() - centerY) + (g->value < 0) * centerY) * MAX_GRAPH_SCALE;

        // The actual y position to draw
        valY = centerLine - valYNormalized * yMultiplier;

        // If this point is closer to the mouse than the current closest, and if the point is not the first or last point
        // Ignore the first and last points for tooltip drawing, because they look bad
        if (fabs(valX - w->m_mousePos.x()) < closestDist && g->time != start && g->time != start + range) {
            closestDist = fabs(valX - w->m_mousePos.x());
            ptLabel = g->value > 5 ? QString::number((int) g->value) : QString::asprintf("%0.2f", g->value);
            highlighted = QPointF(roundf(w->m_mousePos.x()), roundf(valY));
        }

        pointsToDraw.append(QPoint(round(valX), round(valY)));
    }

    bg.setAlphaF(0.4);

    gradient.setFinalStop(rect.topLeft());

    // Middle line
    pointsToDraw.append(QPoint(rect.right(), rect.bottom() - centerY));
    pointsToDraw.append(QPoint(rect.left(), rect.bottom() - centerY));

    graph->polygon->setPolygon(QPolygon(pointsToDraw));
    graph->polygon->setBrush(gradient);
    graph->polygon->setPen(QPen(bg, 2));
    graph->polygon->setZValue(0);

    // Scale the text
    m_widgetFancy.setPointSize(rect.height() * 2 / 3);
    m_widgetFancy.setWeight(QFont::Black);

    graph->textLabel->geometry = rect.adjusted(5, 0, 0, 0);

    graph->textLabel->setFont(m_widgetFancy);
    graph->textLabel->setDefaultTextColor(bg);

    // Now we draw the ticks
    float y = 0;
    // Clamp the scale
    float gScale = fmax(50, fmin(MAX_DYNAMIC_GRAPH_SCALE, scale));

    /*
    // Only need to go half of the way, because graph ticks will be mirrored along the axis
    for (int i = 0; i < gScale/2; i += GRAPH_TICK_DISTANCE)
    {
        // Normalize the value, multiply is by the height of the rectangle, and then scale down a little to fit better
        y = i/gScale * rect.height() * MAX_GRAPH_SCALE;

        // Drawing the actual ticks, 10 pixels from right to left centered about the right side of the rectangle

        QGraphicsLineItem* tick1 = new QGraphicsLineItem(rect.right() - 5, rect.center().y() + y, rect.right(), rect.center().y() + y);
        tick1->setPen(QPen(m_textBrush, 1));
        tick1->setOpacity(0.5);

        QGraphicsLineItem* tick2 = new QGraphicsLineItem(rect.right() - 5, rect.center().y() - y, rect.right(), rect.center().y() - y);
        tick2->setPen(QPen(m_textBrush, 1));
        tick2->setOpacity(0.5);

        //        scene->addItem(tick1);
        //        scene->addItem(tick2);
    }
    */

    // Rectangle that is 50 pixels wide centered around the mouse's x position. Make it the height of the entire rectangle
    // Shift it down by 0.5 before rounding to align things
    ptHighlight = QRect(round(w->m_mousePos.x() - TOOLTIP_WIDTH_HALF), roundf(rect.top() - 0.5), TOOLTIP_WIDTH,
                        roundf(rect.height() + 0.5));

    if (drawTooltip) {
        graph->showTooltip();

        graph->tooltipRect->setRect(ptHighlight);
        graph->tooltipRect->setPen(QPen(m_transparentBrush, 1));
        graph->tooltipRect->setBrush(lightHighlighterBrush);
        graph->tooltipRect->setOpacity(0.1);

        // Draw a line from the top to the bottom of the rectangle, at the x coordinate of the mouse
        // Adjust the top 1 pixel down and the bottom 2 pixels up to make it fit well within the subgraph
        graph->tooltipCenterLine->setLine(w->m_mousePos.x(), roundf(rect.top() + 1), w->m_mousePos.x(),
                                          roundf(rect.bottom() - 2));
        graph->tooltipCenterLine->setOpacity(0.3);
        graph->tooltipCenterLine->setPen(QPen(lightHighlighterBrush, 2));

        bg.setAlphaF(1);
        // Circle at the data point
        graph->tooltipCircle->setRect(highlighted.x() - 2.5, highlighted.y() - 2.5, 5, 5);
        graph->tooltipCircle->setBrush(bg);
        graph->tooltipCircle->setPen(bg);

        graph->tooltipValue->geometry = ptHighlight;
        graph->tooltipValue->setPlainText(ptLabel);
        graph->tooltipValue->setDefaultTextColor(m_textBrush.color());
        graph->tooltipValue->setFont(m_widgetMedium);
    } else         // Only draw current values and graph scale if tooltips are not being shown
    {
        graph->hideTooltip();

        float currentValue = graphPointCircularBufferGetValueAtIndex(data, -1)->value;

        graph->valueLabel->geometry = QRect(rect.right() - 200,
                                            rect.top(),
                                            190,
                                            rect.height());

        graph->valueLabel->setPlainText(
                abs(currentValue) < 10 && abs(currentValue) > 0.01 ? QString::asprintf("%0.2f", currentValue)
                                                                   : QString::asprintf("%d", (int) currentValue));

        graph->valueLabel->setDefaultTextColor(bg);
        graph->valueLabel->setFont(m_widgetFancy);

        m_widgetMedium.setPointSize(rect.height() / 8);

        // Draw text that is the current maximum value for this subgraph. Use text alignment flags to place the text inside of a rectangle that is
        // created to fit the text nicely in the top right corner of the subgraph
        graph->maxValueLabel->geometry = QRect(rect.left(),
                                               rect.top() + 1,
                                               rect.width() - 7,
                                               rect.height() * (1 - MAX_GRAPH_SCALE));

        graph->maxValueLabel->setPlainText(
                abs(maxValue) < 10 && abs(maxValue) > 0.01 ? QString::asprintf("%0.2f", maxValue) : QString::asprintf(
                        "%d", (int) maxValue));
        graph->maxValueLabel->setFont(m_widgetMedium);
        graph->maxValueLabel->setOpacity(0.8);
        graph->maxValueLabel->setDefaultTextColor(m_textBrush.color());


        // The tick denoting the maximum value
        graph->maxTick->setLine(QLine(rect.right() - 5,
                                      rect.top() + rect.height() * (1 - MAX_GRAPH_SCALE) / 2,
                                      rect.right(),
                                      rect.top() + rect.height() * (1 - MAX_GRAPH_SCALE) / 2));

        graph->maxTick->setPen(QPen(m_textBrush, 1));

        // Draw the minimum value
        graph->minValueLabel->geometry = QRect(rect.left(),
                                               rect.bottom() - 1 - rect.height() * (1 - MAX_GRAPH_SCALE),
                                               rect.width() - 7,
                                               rect.height() * (1 - MAX_GRAPH_SCALE));

        graph->minValueLabel->setPlainText(
                abs(minValue) < 10 && abs(minValue) > 0.01 ? QString::asprintf("%0.2f", minValue) : QString::asprintf(
                        "%d", (int) minValue));
        graph->minValueLabel->setFont(m_widgetMedium);
        graph->minValueLabel->setOpacity(0.8);
        graph->minValueLabel->setDefaultTextColor(m_textBrush.color());

        // The tick denoting the minimum value
        graph->minTick->setLine(QLine(rect.right() - 5,
                                      rect.bottom() - rect.height() * (1 - MAX_GRAPH_SCALE) / 2,
                                      rect.right(),
                                      rect.bottom() - rect.height() * (1 - MAX_GRAPH_SCALE) / 2));
        graph->minTick->setPen(QPen(m_textBrush, 1));
    }
}



