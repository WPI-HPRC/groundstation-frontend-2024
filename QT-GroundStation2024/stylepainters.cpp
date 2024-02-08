#include "stylepainters.h"
#include <QApplication>
#include <QStyle>
#include <QPalette>
#include <QStyleOption>
#include <iomanip>
#include <iostream>
#include <QFontDatabase>
#include "Windows/mainwindow.h"
#include <QDateTime>
#include <math.h>
#include <QQuaternion>

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPolygonItem>
#include <QVBoxLayout>

#include "Util/betterqgraphicstextitem.h"
#include "Util/hprcStateMap.h"
#include "Widgets/hprctimeline.h"

#define NUM_NAVBALL_CIRCLES 7

#define MAX_GRAPH_SCALE 0.85
#define GRAPH_TICK_DISTANCE 50
#define MAX_DYNAMIC_GRAPH_SCALE 600


#define MAX_GRAPH_SCALE 0.85
#define GRAPH_TICK_DISTANCE 50
#define MAX_DYNAMIC_GRAPH_SCALE 600

#define TOOLTIP_WIDTH 50
#define TOOLTIP_WIDTH_HALF 25


std::map<double, QString> hprcStateMaps::stateMap;
std::map<HPRCStyle::HPRCAlarmType, bool> hprcStateMaps::alarmMapW;
std::map<HPRCStyle::HPRCAlarmType, bool> hprcStateMaps::alarmMapA;


HPRCStyle::HPRCStyle(const QStyle *style, MainWindow::dataPoint *d)
{
    QPalette widgetPalette = style->standardPalette();
    m_backgroundBrush = widgetPalette.brush(QPalette::Window);
    m_panelBrush = widgetPalette.brush(QPalette::Base);
    m_textBrush = widgetPalette.brush(QPalette::Text);
    m_highlightBrush = widgetPalette.brush(QPalette::Highlight);
    m_buttonBrush = widgetPalette.brush(QPalette::Button);
    m_transparentBrush = *new QBrush(QColor(255, 255, 255, 0));

    int id = QFontDatabase::addApplicationFont(":/fonts/fonts/OverpassMono-VariableFont_wght.ttf");
    QString overpassMono = QFontDatabase::applicationFontFamilies(id).at(0);

    id = QFontDatabase::addApplicationFont(":/fonts/fonts/JockeyOne-Regular.ttf");
    QStringList l = QFontDatabase::applicationFontFamilies(id);
    QString jockeyOne = QFontDatabase::applicationFontFamilies(id).at(0);
    m_widgetFancy = QFont(jockeyOne, 30, 5, false);
    m_widgetLarge = QFont(overpassMono, 20, 5, false);
    m_widgetMedium = QFont(overpassMono, 15, 5, false);
    m_widgetSmall = QFont(overpassMono, 10, 5, false);

    // get a pointer to the current data's location

    m_latest = d;
}

void HPRCStyle::drawPushButton(QPainter *p, const QStyleOption *o)
{

    double borderWidth = 4;

    QPen borderPen(m_highlightBrush, borderWidth);
    p->setRenderHint(QPainter::Antialiasing);
    p->setPen(borderPen);
    p->setBrush(m_buttonBrush);
    p->drawRoundedRect(QRectF(o->rect).adjusted(borderWidth - 0.5, borderWidth - 0.5, 0.5 - borderWidth, 0.5 - borderWidth), o->rect.height()/2, o->rect.height()/2);
}

void HPRCStyle::drawFrame(QPainter *p, const QStyleOption *o)
{
    p->setRenderHint(QPainter::Antialiasing);
    p->setBrush(m_panelBrush);
    p->setPen(QPen(m_transparentBrush, 1));
    p->drawRoundedRect(QRectF(o->rect).adjusted(0.5, 0.5, -0.5, -0.5), 10, 10);

}


void HPRCStyle::drawHPRCViewer(QPainter *p, const hprcDisplayWidget *w)
{
    if (w->getType() == hprcDisplayWidget::HPRC_Viewer) {
        const hprcViewer* viewer = dynamic_cast<const hprcViewer*>(w);
        if (viewer) {
            // Update the orientation of the rocket based on the latest data
            viewer->orientRocket(m_latest->orientation);

            // Resize the 3D viewer to match the container widget
            viewer->m_view->resize(w->size());

            // Update the colors to match the current color scheme
            viewer->updateColors(m_panelBrush.color(), m_highlightBrush.color());
        }
    }
}

void HPRCStyle::drawHPRCTimeline(QPainter *p, const hprcTimeline *w)
{

    if(w->rect().width() < 100)
        return;

    // <------------------- math --------------------->//

    p->setRenderHint(QPainter::Antialiasing);

    p->setFont(m_widgetLarge);
    p->setBrush(m_transparentBrush);
    QPen bgPen(m_backgroundBrush, 15);
    bgPen.setCapStyle(Qt::RoundCap);

    QPen fgPen(m_highlightBrush, 16);
    fgPen.setCapStyle(Qt::RoundCap);

    QPen tickPen(m_textBrush, 20);
    tickPen.setCapStyle(Qt::FlatCap);

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
        textItem->geometry = QRectF(QPointF(w->geometry().left(), ypos - 100), QPointF(topLeftSlot.x() - 20, ypos + 100));

        if(label.startsWith("-"))
        {
            if(!m_latest->timelineActivated[tickIndex]) {
                textItem->setDefaultTextColor(bgPen.color());
            }
            else {

                textItem->setDefaultTextColor(tickPen.color());
            }
            textItem->setFont(m_widgetMedium);
            textItem->setPos(p->font().pointSize() * -8 + 20, 0);
            textItem->setPlainText(m_latest->timelineTimes[tickIndex]);
            tickIndex++;
        }
    }

    w->graphicsView->viewport()->update();

}

void HPRCStyle::drawHPRCGauge(QPainter *p, const hprcDisplayWidget *w)
{
    if (w->rect().height() < 100)
        return;

    p->setRenderHint(QPainter::Antialiasing);
    p->setBrush(m_backgroundBrush);
    QPen bgPen(m_backgroundBrush, 5);
    bgPen.setCapStyle(Qt::RoundCap);

    QPen textPen(m_textBrush, 5);

    QRectF boundingBox(w->rect().adjusted(15, 15, -15, -15));

    double scaleF = 0.85;

    int oWidth = boundingBox.width();

    int sizeMin = fmin(boundingBox.height(), boundingBox.width() * scaleF);

    boundingBox.adjust(oWidth/2 - sizeMin/2, 0, oWidth/2 - sizeMin/2, 0);

    boundingBox.setHeight(sizeMin);
    boundingBox.setWidth(sizeMin);

    int extraArc = 35;


    QConicalGradient progressGradient(boundingBox.center(), (180 + extraArc - 5) - (w->m_filledPercent/w->m_max) * (180 + 2*extraArc));
    progressGradient.setColorAt(1, m_panelBrush.color());
    progressGradient.setColorAt(0, m_highlightBrush.color());

    QPen fgPen(m_highlightBrush, 5);

    fgPen.setCapStyle(Qt::RoundCap);

    bgPen.setWidth(sizeMin/10);
    fgPen.setWidth(sizeMin/10 -5);

    QString dataString = QString::number(w->m_filledPercent);
    bool negative = false;
    if(dataString.contains("-"))
    {
        dataString.remove("-");
        negative = true;
    }
    while(dataString.size() < 4)
    {
        dataString.prepend("0");
    }
    if(negative)
        dataString.prepend("-");

    m_widgetLarge.setPointSize(sizeMin/13);
    p->setFont(m_widgetLarge);

    // <---- draw ----> //

    p->setPen(bgPen);
    p->drawArc(boundingBox, -extraArc*16, (180 + 2 * extraArc)*16);
    p->setPen(fgPen);
    p->drawArc(boundingBox, (180 * 16 + extraArc*16), (180 + 2 * extraArc)*-16 * (fmax(0, fmin(w->m_filledPercent/w->m_max, 1))));
    p->setPen(textPen);
    p->drawText(boundingBox.adjusted(0, 30, 0, 30), Qt::AlignCenter, w->m_label);

    m_widgetLarge.setPointSize(sizeMin/7);
    p->setFont(m_widgetLarge);

    p->drawText(boundingBox.adjusted(0, -30, 0, -30), Qt::AlignCenter, dataString);
}

void HPRCStyle::drawHPRCAttitudeWidget(QPainter *p, const hprcDisplayWidget *w)
{
    if(w->width() < 100)
        return;

    // -- Pen Setup --

    p->setRenderHint(QPainter::Antialiasing);
    p->setBrush(m_backgroundBrush);

    QPen bgPen(m_backgroundBrush, 5);

    bgPen.setCapStyle(Qt::RoundCap);

    // -- Create bounding box --

    QRectF boundingBox(w->rect().adjusted(15, 15, -15, -15));

    double scaleF = 0.8;

    int sizeMin = fmin(boundingBox.height() * scaleF, boundingBox.width() * scaleF);

    boundingBox.adjust(boundingBox.width()/2 - sizeMin/2,
                       boundingBox.height()/2 - sizeMin/2,
                       boundingBox.width()/2 - sizeMin/2,
                       boundingBox.height()/2 - sizeMin/2
                       );

    boundingBox.setHeight(sizeMin);
    boundingBox.setWidth(sizeMin);

    // For testing purposes

    /*
    QQuaternion quat(1, ((w->m_mousePos.y() - boundingBox.y()) / boundingBox.height() - 0.5) * 2,
                     ((w->m_mousePos.x() - boundingBox.x()) /boundingBox.width() - 0.5) * 2, 0);
*/

    float crossWidth = sizeMin/10;
    crossWidth /= 2;

    bgPen.setWidth(crossWidth);

    // -- Get the angles and use them --

    float pitch, yaw, roll;
    m_latest->orientation.getEulerAngles(&pitch, &yaw, &roll);
    roll = m_latest->gyroZ;
//    quat.getEulerAngles(&pitch, &yaw, &roll);

    // Clamp to values
    pitch = fminf(m_AttitudeDegreeOffsetPitch + m_AttitudeMaxDegreeRange, fmaxf(m_AttitudeDegreeOffsetPitch - m_AttitudeMaxDegreeRange, pitch));
    yaw = fminf(m_AttitudeDegreeOffsetYaw + m_AttitudeMaxDegreeRange, fmaxf(m_AttitudeDegreeOffsetYaw - m_AttitudeMaxDegreeRange, yaw));

    // Normalize between -1 -> +1
    float pitchNormalized = (pitch - m_AttitudeDegreeOffsetPitch)/m_AttitudeMaxDegreeRange;
    float yawNormalized = (yaw - m_AttitudeDegreeOffsetYaw)/m_AttitudeMaxDegreeRange;

    float yawY = boundingBox.center().y() + (boundingBox.height()/2 - crossWidth*2) * -1 * pitchNormalized;
    float pitchX = boundingBox.center().x() + (boundingBox.width()/2 - crossWidth*2) * yawNormalized;


    // -- Draw the info --

    // Center dot

    p->setPen(QPen(m_textBrush, 2));

    float circleLocation;

    std::vector<float> vec = m_AttitudeCircleLocationsDegrees[m_latest->state];

    bool rocketIsWithinGraph = pitch * pitch + yaw * yaw < vec.back() * vec.back();

    std::reverse(vec.begin(), vec.end());

    QColor bgCol = bgPen.color();
    bgCol.setAlphaF(0.5);

    QPen outline = QPen(QBrush(bgCol), 4);

    QColor fgCol = m_highlightBrush.color();

    for(float circleLocationDegrees: vec) {

        circleLocation = circleLocationDegrees / (m_AttitudeMaxDegreeRange);

        float dist = hypot(yaw, pitch) - circleLocationDegrees;

        p->setPen(outline);

        if(rocketIsWithinGraph){
            fgCol.setAlphaF(fminf(
                powf(1 - fabsf(dist)/m_AttitudeMaxDegreeRange, 4),
                powf((dist + circleLocationDegrees)/m_AttitudeMaxDegreeRange, 3)));
        }
        else {
            fgCol.setAlphaF(1);
        }

        p->setBrush(QBrush(fgCol));


        p->drawEllipse(roundf(boundingBox.center().x()-boundingBox.width()/2*circleLocation),
                       roundf(boundingBox.center().y()-boundingBox.width()/2*circleLocation),
                       roundf(boundingBox.width()*circleLocation), roundf(boundingBox.width()*circleLocation));
    }

    // Crosshair
    p->setOpacity(1);
    p->setPen(QPen(m_textBrush, 2));
    QPoint center((int)pitchX, (int)yawY);

    p->drawLine(center.x() - 4, center.y(), center.x() + 4, center.y());
    p->drawLine(center.x(), center.y() - 4, center.x(), center.y() + 4);

    if(rocketIsWithinGraph)
        p->setPen(QPen(m_textBrush, 2));
    else
        p->setPen(QPen(m_highlightBrush, 2));

    m_widgetLarge.setPointSize(crossWidth*3);

    p->setFont(m_widgetLarge);

    m_widgetLarge.setPointSize(crossWidth*2);


    p->setPen(QPen(m_textBrush, 2));
    p->setFont(m_widgetLarge);

        p->drawText(QRect(boundingBox.x()-20 - crossWidth*3,
                          boundingBox.y()-30,
                          crossWidth * 10,
                          crossWidth * 3
                          ),
                    Qt::AlignVCenter,
                    "PITCH");

        m_widgetLarge.setPointSize(crossWidth * 2);
        p->setFont(m_widgetLarge);

        p->drawText(QRect(boundingBox.x() - 20,
                          boundingBox.y() + boundingBox.height() + 17,
                          boundingBox.width() + 40,
                          crossWidth * 3
                          ),
                    Qt::AlignCenter,
                    QString::asprintf("ROLL RATE: %.0lf rpm", roll)); // TODO: change this to the actual value

        m_widgetLarge.setPointSize(crossWidth*2);

        p->setFont(m_widgetLarge);

    if(rocketIsWithinGraph)
        p->setPen(QPen(m_textBrush, 2));
    else
        p->setPen(QPen(m_highlightBrush, 2));

    p->drawText(QRect(boundingBox.x()-20 - crossWidth*3,
                      boundingBox.y()-30 + crossWidth*3,
                      crossWidth * 6,
                      crossWidth * 3
                      ),
                Qt::AlignCenter,
                QString::asprintf("%.0lfº", pitch));


    p->setPen(QPen(m_textBrush, 2));

    p->drawText(QRect(boundingBox.x() + boundingBox.width() + 20 - crossWidth*5,
                      boundingBox.y()-30,
                      crossWidth * 10,
                      crossWidth * 3
                      ),
                Qt::AlignCenter,
                "YAW");

    if(rocketIsWithinGraph)
        p->setPen(QPen(m_textBrush, 2));
    else
        p->setPen(QPen(m_highlightBrush, 2));
    p->drawText(QRect(boundingBox.x() + boundingBox.width() + 20 - crossWidth*3,
                      boundingBox.y()-30 + crossWidth*3,
                      crossWidth * 6,
                      crossWidth * 3
                      ),
                Qt::AlignCenter,
                QString::asprintf("%.0lfº", yaw));

}

void HPRCStyle::drawHPRCGraph(QPainter *p, hprcGraph *w)
{
    int width= w->rect().width();
    int height = w->rect().height();

    double scaleF = 0.75;
    double paddingRatio = (1-scaleF) / 2.0;

    int margin = fmin(paddingRatio * width, paddingRatio * height);

    QRectF drawBox = w->layout()->geometry().adjusted(margin, margin, -margin, -margin);

    w->graphicsView->setSceneRect(w->layout()->geometry());

    // label padding = 7.5%
    int lMargin = drawBox.height() * 0.075;
    drawBox.adjust(lMargin, 0, 0, -lMargin);

    QPointF topLeftTop = drawBox.topLeft();
    QPointF bottomRightBottom = drawBox.bottomRight();

    int row2Top = drawBox.y() + drawBox.height()/3;
    int row3Top = drawBox.y() + 2 * drawBox.height()/3;

    QPointF bottomRightTop(drawBox.right(), row2Top);
    QPointF topLeftMiddle(drawBox.left(), row2Top);
    QPointF bottomRightMiddle(drawBox.right(), row3Top);
    QPointF topLeftBottom(drawBox.left(), row3Top);

    QRectF top(topLeftTop, bottomRightTop);
    QRectF middle(topLeftMiddle, bottomRightMiddle);
    QRectF bottom(topLeftBottom, bottomRightBottom);

    double start = graphPointCircularBufferGetValueAtIndex(m_latest->altData, 0)->time;
    double range = graphPointCircularBufferGetValueAtIndex(m_latest->altData, m_latest->altData->length - 1)->time - start;


    bool drawT = false;
    if(drawBox.contains(w->m_mousePos))
    {
        drawT = true;

        w->m_mousePos.setX(fmin(fmax(w->m_mousePos.x(), drawBox.x() + TOOLTIP_WIDTH_HALF + 2), drawBox.right() - TOOLTIP_WIDTH_HALF - 2));
    }



    // <---- draw ----> //

    w->graphicsScene->setBackgroundBrush(m_transparentBrush);

    // Do a little adjusting to help with tooltip rendering
    w->bgRect->setRect(drawBox.adjusted(0, 0, 0, 2));
    w->bgRect->setPen(QPen(m_backgroundBrush, 6));
    w->bgRect->setBrush(m_backgroundBrush);
    w->bgRect->setZValue(-1);

    // Do a little adjusting to help with tooltip rendering
    drawHPRCSubGraph(p, top, m_highlightBrush.color(), m_latest->accData, GRAPH_Acceleration, range, start, w, w->graphicsScene, drawT);
    drawHPRCSubGraph(p, middle.adjusted(0, 1, 0, 1), QColor("#2c4985"), m_latest->velData, GRAPH_Velocity, range, start, w, w->graphicsScene, drawT);
    drawHPRCSubGraph(p, bottom.adjusted(0, 2, 0, 2), QColor("#471d57"), m_latest->altData, GRAPH_Altitude, range, start, w, w->graphicsScene, drawT);

    // Draw an outline to clean up weird border rendering
    w->outlineRect->setRect(drawBox.adjusted(0, 0, 0, 3));
    w->outlineRect->setBrush(m_transparentBrush);
    w->outlineRect->setPen(QPen(m_backgroundBrush, 4));
    w->outlineRect->setZValue(100);

    w->graphicsView->viewport()->update();
}

void HPRCStyle::drawHPRCSubGraph(QPainter *p, QRectF rect, QColor bg, GraphPointCircularBuffer *data, GraphType graphType,  double range, double start, hprcGraph *w, QGraphicsScene* scene, bool drawTooltip)
{
    if (rect.height() < 10)
        return;

    HPRCSubGraph* graph;

    switch (graphType)
    {
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
    QRectF ptHighlight(-100,0,0,0);
    QPointF highlighted(-100,0);
    QString ptLabel("");

    bool drawMaxMarker = true;

    double centerY = 0;

    // Setting the centerpoint of the axis
    if(scaleMin < 0)
    {
        // Move the centerline up away from the bottom of the rectangle so negative points fit on the graph
        centerY = fabs(scaleMin/scale) * rect.height();
        scaleMin = 0;
    }
    else
    {
        scale = scaleMax;
    }

    double valYNormalized = 0;
    double yMultiplier = 0;

    double valX = 0;
    double valY = 0;

    double centerLine = rect.top() + rect.height() - centerY;

    double closestDist = std::numeric_limits<double>::max();


    for (int i = 0; i < CIRCULAR_BUFFER_LEN; i++)
    {
        GraphPoint *g = graphPointCircularBufferGetValueAtIndex(data, i);
        valX = rect.left() + 1 + (g->time - start) / range * (rect.width()-1); // This was Ben's math. It works

        valYNormalized = g->value / scale;

        // Multiply the normalized value by the distance between the center line and the top or the bottom
        // depending on whether the value is positive or negative. Use some boolean algebra to avoid an if statement
        yMultiplier = ((g->value > 0) * (rect.height() - centerY) + (g->value < 0) * centerY) * MAX_GRAPH_SCALE;

        // The actual y position to draw
        valY = centerLine - valYNormalized * yMultiplier;

        // If this point is closer to the mouse than the current closest, and if the point is not the first or last point
        // Ignore the first and last points for tooltip drawing, because they look bad
        if(fabs(valX - w->m_mousePos.x()) < closestDist && g->time !=start && g->time != start+range)
        {
            closestDist = fabs(valX - w->m_mousePos.x());
            ptLabel = g->value > 5 ? QString::number((int)g->value) : QString::asprintf("%0.2f", g->value);
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
    m_widgetFancy.setPointSize(rect.height() * 2/3);
    m_widgetFancy.setWeight(QFont::Black);

    graph->textLabel->geometry = rect.adjusted(5, 0, 0, 0);

    graph->textLabel->setFont(m_widgetFancy);
    graph->textLabel->setDefaultTextColor(bg);

    // Now we draw the ticks
    float y = 0;
    // Clamp the scale
    float gScale = fmax(50, fmin(MAX_DYNAMIC_GRAPH_SCALE, scale));

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

    // Rectangle that is 50 pixels wide centered around the mouse's x position. Make it the height of the entire rectangle
    // Shift it down by 0.5 before rounding to align things
    ptHighlight = QRect(round(w->m_mousePos.x() - TOOLTIP_WIDTH_HALF), roundf(rect.top() - 0.5), TOOLTIP_WIDTH, roundf(rect.height() + 0.5));

    if(drawTooltip)
    {
        graph->showTooltip();

        graph->tooltipRect->setRect(ptHighlight);
        graph->tooltipRect->setPen(QPen(m_transparentBrush, 1));
        graph->tooltipRect->setBrush(lightHighlighterBrush);
        graph->tooltipRect->setOpacity(0.1);

        // Draw a line from the top to the bottom of the rectangle, at the x coordinate of the mouse
        // Adjust the top 1 pixel down and the bottom 2 pixels up to make it fit well within the subgraph
        graph->tooltipCenterLine->setLine(w->m_mousePos.x(), roundf(rect.top() + 1), w->m_mousePos.x(), roundf(rect.bottom() - 2));
        graph->tooltipCenterLine->setOpacity(0.3);
        graph->tooltipCenterLine->setPen(QPen(lightHighlighterBrush, 2));

        bg.setAlphaF(1);
        // Circle at the data point
        graph->tooltipCircle->setRect(highlighted.x() - 2.5, highlighted.y() - 2.5, 5, 5);
        graph->tooltipCircle->setBrush(bg);
        graph->tooltipCircle->setPen(bg);

        graph->tooltipValue->geometry = ptHighlight;
        graph->tooltipValue->text = ptLabel;
        graph->tooltipValue->setDefaultTextColor(m_textBrush.color());
        graph->tooltipValue->setFont(m_widgetMedium);
    }
    else         // Only draw current values and graph scale if tooltips are not being shown
    {
        graph->hideTooltip();

        float currentValue = graphPointCircularBufferGetValueAtIndex(data, -1)->value;

        graph->valueLabel->geometry = QRect(rect.right() - 200,
                                            rect.top(),
                                            190,
                                            rect.height());

        graph->valueLabel->text = abs(currentValue) < 10 && abs(currentValue) > 0.01 ? QString::asprintf("%0.2f", currentValue) : QString::asprintf("%d", (int)currentValue);

        graph->valueLabel->setDefaultTextColor(bg);
        graph->valueLabel->setFont(m_widgetFancy);

        m_widgetMedium.setPointSize(rect.height()/8);

        // Draw text that is the current maximum value for this subgraph. Use text alignment flags to place the text inside of a rectangle that is
        // created to fit the text nicely in the top right corner of the subgraph
        graph->maxValueLabel->geometry = QRect(rect.left(),
                                               rect.top()+1,
                                               rect.width() - 7,
                                               rect.height()*(1-MAX_GRAPH_SCALE));

        graph->maxValueLabel->text = abs(maxValue) < 10 && abs(maxValue) > 0.01 ? QString::asprintf("%0.2f", maxValue) : QString::asprintf("%d", (int)maxValue);
        graph->maxValueLabel->setFont(m_widgetMedium);
        graph->maxValueLabel->setOpacity(0.8);
        graph->maxValueLabel->setDefaultTextColor(m_textBrush.color());


        // The tick denoting the maximum value
        graph->maxTick->setLine(QLine(rect.right() - 5,
                                      rect.top() + rect.height()*(1-MAX_GRAPH_SCALE)/2,
                                      rect.right(),
                                      rect.top() + rect.height()*(1-MAX_GRAPH_SCALE)/2));

        graph->maxTick->setPen(QPen(m_textBrush, 1));

        // Draw the minimum value
        graph->minValueLabel->geometry = QRect(rect.left(),
                                               rect.bottom()-1-rect.height()*(1-MAX_GRAPH_SCALE),
                                               rect.width() - 7,
                                               rect.height()*(1-MAX_GRAPH_SCALE));

        graph->minValueLabel->text = abs(minValue) < 10 && abs(minValue) > 0.01 ? QString::asprintf("%0.2f", minValue) : QString::asprintf("%d", (int)minValue);
        graph->minValueLabel->setFont(m_widgetMedium);
        graph->minValueLabel->setOpacity(0.8);
        graph->minValueLabel->setDefaultTextColor(m_textBrush.color());

        // The tick denoting the minimum value
        graph->minTick->setLine(QLine(rect.right() - 5,
                                      rect.bottom() - rect.height()*(1-MAX_GRAPH_SCALE)/2,
                                      rect.right(),
                                      rect.bottom() - rect.height()*(1-MAX_GRAPH_SCALE)/2));
        graph->minTick->setPen(QPen(m_textBrush, 1));
    }
}

void HPRCStyle::drawHPRCAlarmPanel(QPainter *p, const hprcDisplayWidget *w)
{
    p->setRenderHint(QPainter::Antialiasing);
    p->setBrush(m_transparentBrush);
    QPen linePen(m_backgroundBrush, 4);
    linePen.setCapStyle(Qt::RoundCap);
    p->setPen(linePen);

    int abortConditions = hprcStateMaps::alarmMapA.size();
    int warnConditions = hprcStateMaps::alarmMapW.size();

    int total = abortConditions + warnConditions + 1;

    double width = w->rect().width() * 0.9;
    double height = w->rect().height() * 0.9;

    double aspectRatio = (1.0/total);
    if(width / height > aspectRatio)
    {
        width = height * aspectRatio;
    } else {
        height = width / aspectRatio;
    }

    double wPadding = (w->rect().width() - width) / 2.0;
    double hPadding = (w->rect().height() - height) / 2.0;

    int i = 0;
    int size = width;
    int x = w->rect().x() + wPadding;

    int startX = x + size * 0.05;
    int startYA = w->rect().y() + hPadding + size * 0.5;

    p->drawLine(startX, startYA, startX, w->rect().y() + hPadding + (height * (abortConditions - 1) / total + size * 0.5));

    for(const auto& [type, active] : hprcStateMaps::alarmMapA)
    {
        int y = w->rect().y() + hPadding + (height * i / total);
        drawHPRCAlarmFromEnum(p, x, y, size, type, active, startX, startYA);

        i++;
    }
    i++;
    p->setPen(linePen);
    int startYW = w->rect().y() + hPadding + (height * i / total + size * 0.5);
    p->drawLine(startX, startYW, startX, startYW + (size * (warnConditions - 1)));


    for(const auto& [type, active] : hprcStateMaps::alarmMapW)
    {
        int y = w->rect().y() + hPadding + (height * i / total);
        drawHPRCAlarmFromEnum(p, x, y, size, type, active, startX, startYW);

        i++;
    }
    p->setPen(linePen);
}

void HPRCStyle::drawHPRCAlarmFromEnum(QPainter *p, int x, int y, int size, HPRCAlarmType t, bool active, int startX, int startY)
{
    p->setBrush(m_panelBrush);
    if(active)
    {
        QPen hPen(m_highlightBrush, 4);
        hPen.setCapStyle(Qt::RoundCap);
        p->setPen(hPen);
        p->drawLine(x + size * 0.05, y + size * 0.5, startX, startY);
    } else {
        QPen hPen(m_backgroundBrush, 4);
        hPen.setCapStyle(Qt::RoundCap);
        p->setPen(hPen);
    }

    // <---- draw the "causation" line ----> //

    double radius = 0.05;

    p->drawLine(x + size * 0.05, y + size * 0.5, x + size * 0.25, y + size * 0.5);
    QRectF circleRect(x + size * 0.25, y + size * (0.5-radius), radius * 2 * size, radius * 2 * size);
    p->drawEllipse(circleRect);

    // <---- draw the icon ----> //

    double iconScale = 0.5;
    double iconSize = iconScale * size;
    double iconX = x + (size - iconSize) / 1.15; // shifted right to account for space used by "causation line"
    double iconY = y + (size - iconSize) / 2.0;

    QRectF iRect(iconX, iconY, iconSize, iconSize);

    p->drawRect(iRect);


    switch(t)
    {
    case ALARM_MasterAbort:
    {

        return;
    }
    case ALARM_Pitch:
    case ALARM_MasterWarn:
    case ALARM_LowPower:
    case ALARM_SignalLoss:
    case ALARM_PowerLoss:
    case ALARM_MainDeployFail:
    case ALARM_DrogueDeployFail:
    case ALARM_PayloadDeployFail:
    case ALARM_Ballistic:
    case ALARM_LowTrack:
    case ALARM_HighTrack:
    case ALARM_EarlyChute:
    case ALARM_Unknown:
        break;
    }
}

void HPRCStyle::drawHPRCClock(QPainter *p, const hprcDisplayWidget *w)
{
    m_widgetLarge.setPointSize(w->height()/5);
    p->setFont(m_widgetLarge);
    p->setPen(QPen(m_textBrush, 3));
    if(w->getDataType() == hprcDisplayWidget::HPRC_MET)
    {
        double t = m_latest->groundTime / 1000.0;

        int wholeNumSecs = t;
        int hours = wholeNumSecs / 60 / 60;
        int minutes = (wholeNumSecs - hours * 60 * 60) / 60;
        double seconds = (t - hours * 60.0 * 60.0 - minutes * 60.0);

        int rseconds = seconds * 100;
        seconds = rseconds/100.0;

        using namespace std;

        string hourString(to_string(hours));
        string minuteString(to_string(minutes));
        string secondString(to_string(seconds));

        if(hourString.length() == 1)
            hourString.insert(0, "0");
        if(minuteString.length() == 1)
            minuteString.insert(0, "0");
        if(secondString.at(1) == *".")
            secondString.insert(0, "0");
        secondString = secondString.substr(0, 5);
        string timeString = "Ground Time: T + " + hourString + ":" + minuteString + ":" + secondString;
        p->drawText(w->rect().adjusted(0,0,0,w->rect().height()/-2.0), QString::fromStdString(timeString));

        // rocket internal time //

        t = m_latest->rocketTimeSinceLaunch / 1000.0;

        wholeNumSecs = t;
        hours = wholeNumSecs / 60 / 60;
        minutes = (wholeNumSecs - hours * 60 * 60) / 60;
        seconds = (t - hours * 60.0 * 60.0 - minutes * 60.0);

        rseconds = seconds * 100;
        seconds = rseconds/100.0;

        using namespace std;

        string hourString2(to_string(hours));
        string minuteString2(to_string(minutes));
        string secondString2(to_string(seconds));

        if(hourString2.length() == 1)
            hourString2.insert(0, "0");
        if(minuteString2.length() == 1)
            minuteString2.insert(0, "0");
        if(secondString2.at(1) == *".")
            secondString2.insert(0, "0");
        secondString2 = secondString2.substr(0, 5);
        string timeString2 = "Rocket Time: T + " + hourString2 + ":" + minuteString2 + ":" + secondString2;
        p->drawText(w->rect().adjusted(0,w->rect().height()/2.0, 0, 0), QString::fromStdString(timeString2));
    } else if(w->getDataType() == hprcDisplayWidget::HPRC_UTC){


        QDateTime currentUTC = QDateTime::currentDateTimeUtc();
        p->drawText(w->rect(), currentUTC.toString("hh:mm:ss dddd, MMMM dd yyyy"));


    }



}

void HPRCStyle::drawHPRCRocketVis(QPainter *p, const hprcDisplayWidget *w)
{

    if(w->width() < 100)
        return;

    // setup
    p->setBrush(m_transparentBrush);

    p->setPen(QPen(m_highlightBrush, 3));
    p->setRenderHint(QPainter::Antialiasing, true);

    // outer possible drawing area - unknown aspect ratio
    QRectF widgetBoundingBox = w->rect();
    double boundH = widgetBoundingBox.height();
    double boundW = widgetBoundingBox.width();

    double newW;
    double newH;

    // desired aspect ratio: 3:1

    if(boundW / boundH > 3.0)
    {
        newW = boundH * 3.0;
        newH = boundH;
    } else {
        newH = boundW / 3.0;
        newW = boundW;
    }

    // the outer bounding box of the drawable area - known aspect ratio
    QRectF drawBox = widgetBoundingBox.adjusted((boundW - newW) / 2.0, (boundH - newH) / 2.0, -(boundW - newW) / 2.0, -(boundH - newH) / 2.0);

    // the box containing the rocket display itself
    QRectF rocketBox = drawBox.adjusted(newW * 0.1, newH * 0.3, newW * -0.1, newH * -0.3);

    // debug
//    p->drawRect(drawBox);
//    p->drawRect(rocketBox);

    // draw first layer - rocket outline
    QPen tPen(m_textBrush, 3);
    tPen.setCapStyle(Qt::RoundCap);
    p->setPen(tPen);
    QRectF rocketBody = rocketBox.adjusted(rocketBox.width() * 0.05, rocketBox.height() * 0.325, rocketBox.width() * -0.2, rocketBox.height() * -0.325);
    QPointF upperFin[4];
    upperFin[0] = QPointF(rocketBody.left(), rocketBox.top());
    upperFin[1] = QPointF(rocketBody.left() + rocketBody.width() * 0.075, rocketBox.top());
    upperFin[2] = QPointF(upperFin[1].x() + (rocketBox.height() - rocketBody.height())/2.0, rocketBody.top());
    upperFin[3] = QPointF(rocketBody.topLeft());
    QPointF lowerFin[4];
    lowerFin[0] = QPointF(rocketBody.left(), rocketBox.bottom());
    lowerFin[1] = QPointF(rocketBody.left() + rocketBody.width() * 0.075, rocketBox.bottom());
    lowerFin[2] = QPointF(lowerFin[1].x() + (rocketBox.height() - rocketBody.height())/2.0, rocketBody.bottom());
    lowerFin[3] = QPointF(rocketBody.bottomLeft());
    QPointF tailCone[4];
    tailCone[0] = QPointF(rocketBody.topLeft());
    tailCone[1] = QPointF(rocketBody.bottomLeft());
    tailCone[2] = QPointF(rocketBox.left(), rocketBody.bottom() + rocketBody.height() * -0.1);
    tailCone[3] = QPointF(rocketBox.left(), rocketBody.top() + rocketBody.height() * 0.1);


    // precalculated trigonometry
    double h = 2.294742 * rocketBox.width();
    double v = 262.791238149 * rocketBody.height();

    QRectF noseConeRectU(QPointF(rocketBody.right() - h, rocketBody.top()), QPointF(rocketBody.right() + h, rocketBody.top() + v));
    QRectF noseConeRectL(QPointF(rocketBody.right() - h, rocketBody.bottom()), QPointF(rocketBody.right() + h, rocketBody.bottom() - v));

    p->drawPolygon(upperFin, 4);
    p->drawPolygon(lowerFin, 4);
    p->drawPolygon(tailCone, 4);
    p->drawArc(noseConeRectU, 90 * 16, -5 * 16);
    p->drawArc(noseConeRectL, 270 * 16, 5 * 16);
    p->drawRect(rocketBody);

    // draw second layer - rocket components

    QRectF airbrakes(rocketBody.adjusted(rocketBody.width() * 0.3, 0, rocketBody.width() * -0.65, 0));
    QRectF avionicsBay(rocketBody.adjusted(rocketBody.width() * 0.35, 0, rocketBody.width() * -0.45, 0));

    p->drawRect(airbrakes);
    p->drawRect(avionicsBay);

    m_widgetLarge.setPointSize(rocketBody.height() / 3);
    drawHPRCRocketLabel(p, LABEL_Motor, QPointF(upperFin[2].x() + rocketBody.width() * 0.05, rocketBody.top() - rocketBody.height() * 0.25), QPointF(upperFin[2].x() + rocketBody.width() * 0.1, rocketBody.top() - rocketBody.height() * 1));
    drawHPRCRocketLabel(p, LABEL_Airbrakes, QPointF(rocketBody.x() + rocketBody.width() * 0.325, rocketBody.bottom() + rocketBody.height() * 0.25), QPointF(rocketBody.x() + rocketBody.width() * 0.325 - rocketBody.width() * 0.1, rocketBody.bottom() + rocketBody.height() * 1.75));
    drawHPRCRocketLabel(p, LABEL_AvionicsBay, QPointF(rocketBody.x() + rocketBody.width() * 0.45, rocketBody.top() - rocketBody.height() * 0.25), QPointF(rocketBody.x() + rocketBody.width() * 0.45 + rocketBody.width() * 0.075, rocketBody.top() - rocketBody.height() * 2));
    drawHPRCRocketLabel(p, LABEL_RecoveryBay, QPointF(rocketBody.x() + rocketBody.width() * 0.75, rocketBody.top() - rocketBody.height() * 0.25), QPointF(rocketBody.x() + rocketBody.width() * 0.75 + rocketBody.width() * 0.075, rocketBody.top() - rocketBody.height() * 1));
    drawHPRCRocketLabel(p, LABEL_PayloadBay, QPointF(rocketBody.x() + rocketBody.width() * 1.15, rocketBody.bottom() + rocketBody.height() * 0.25), QPointF(rocketBody.x() + rocketBody.width() * 1.15 - rocketBody.width() * 0.075, rocketBody.bottom() + rocketBody.height() * 1.25));

}

void HPRCStyle::drawHPRCRocketLabel(QPainter *p, rocketLabel l, QPointF target, QPointF label)
{
    // setup
    QPen tPen(m_textBrush, 3);
    tPen.setCapStyle(Qt::RoundCap);
    p->setPen(tPen);
    p->setBrush(m_transparentBrush);
    p->setFont(m_widgetLarge);


    bool drawLeft = target.x() - label.x() > 0;

    p->drawLine(target, label);

    QPointF labelEnd;
    QRectF labelMainText;
    QRectF labelSubText;
    QString mainText;
    switch(l)
    {

    case LABEL_Motor:
        mainText = QString("MOTOR");
        break;
    case LABEL_Airbrakes:
        mainText = QString("AIRBRAKES");
        break;
    case LABEL_PayloadBay:
        mainText = QString("PAYLOAD_BAY");
        break;
    case LABEL_AvionicsBay:
        mainText = QString("AV_BAY");
        break;
    case LABEL_RecoveryBay:
        mainText = QString("RECOVERY_BAY");
        break;
    case LABEL_Other:
        break;
    }

    if(drawLeft)
    {
        labelEnd = QPointF(label.x() - m_widgetLarge.pointSizeF() * mainText.length() * 0.85, label.y());
        labelMainText = QRectF(QPointF(labelEnd.x(), labelEnd.y() - m_widgetLarge.pointSizeF() * 1.45), QPointF(label.x(), label.y() - m_widgetLarge.pointSizeF() * 0.15));
    } else {
        labelEnd = QPointF(label.x() + m_widgetLarge.pointSizeF() * mainText.length() * 0.85, label.y());
        labelMainText = QRectF(QPointF(label.x(), label.y() - m_widgetLarge.pointSizeF() * 1.45), QPointF(labelEnd.x(), labelEnd.y() - m_widgetLarge.pointSizeF() * 0.15));

    }

    p->drawLine(target, label);
    p->drawLine(label, labelEnd);

    p->drawText(labelMainText, Qt::AlignCenter, mainText);


}

void HPRCStyle::drawHPRCAirbrakes(QPainter *p, const hprcDisplayWidget *w)
{
    /*
    if(w->width() < 100)
        return;
*/
    p->setRenderHint(QPainter::Antialiasing);

    QPen textPenRed = QPen(m_highlightBrush, 3);
    QPen textPenWhite = QPen(m_textBrush, 3);
    QPen rectPen = QPen(m_textBrush, w->height() / 50); //Width of rectangles depends on height
    rectPen.setCapStyle(Qt::RoundCap);
    QPen rectLightPen = QPen(m_highlightBrush, w->height() / 50);
    rectLightPen.setCapStyle(Qt::RoundCap);
    QBrush circleBrush = m_backgroundBrush;

    std::string currentText = "Current: " + std::to_string((int) round(m_latest->currentAirbrakes * 100)) + "%";
    std::string desiredText = "Desired: " + std::to_string((int) round(m_latest->desiredAirbrakes * 100)) + "%";

//    std::cout << currentText << std::endl;

    m_widgetLarge.setPointSize(w->width()/20);
    p->setFont(m_widgetLarge);
    float currentTextHeight = QFontMetrics(p->font()).lineSpacing(); //Get height of current text
    float desiredTextHeight = QFontMetrics(p->font()).lineSpacing();

    float circleRadius = w->height() / 4;
    float airbrakeSquareLength = circleRadius / 1.6; //The square length is the length such that the corners of all the squares are touching at the zero position (the 1.6 is just a guess)
    QPointF circlePosition = QPointF(w->height() / 2, w->height() / 2);

    //Draw text
    p->setPen(textPenWhite);
    p->drawText(QRect(circlePosition.x() * 2, w->height() / 2 - currentTextHeight,
                    w->width() - circlePosition.x() * 2, w->height()),
                    Qt::AlignHCenter, QString::fromStdString(currentText));
    p->setPen(textPenRed);
    p->drawText(QRect(circlePosition.x() * 2, w->height() / 2,
                    w->width() - circlePosition.x() * 2, w->height()),
                    Qt::AlignHCenter, QString::fromStdString(desiredText));

    //p->drawRect(QRect(circlePosition.x() * 2, w->rect().height() / 2 - currentTextHeight, w->rect().width() - circlePosition.x() * 2, w->rect().height()));

    float brakeZeroPosition = sin(acos((airbrakeSquareLength / 2) / circleRadius)) * circleRadius; //Distance from center of circle where the squares should be when airbrakes are zero
    float currentPosition = m_latest->currentAirbrakes * airbrakeSquareLength;
    float desiredPosition = m_latest->desiredAirbrakes * airbrakeSquareLength;
    //All the rectangle positions are relative to the center of the circle because of translations when they are drawn
    QList<QRect> airbrakeRectanglesCurrent = {
        QRect(-airbrakeSquareLength / 2.0, -brakeZeroPosition - currentPosition,
              airbrakeSquareLength, airbrakeSquareLength),
        QRect(-brakeZeroPosition - currentPosition, -airbrakeSquareLength / 2.0,
              airbrakeSquareLength, airbrakeSquareLength),
        QRect(-airbrakeSquareLength / 2.0, -airbrakeSquareLength + brakeZeroPosition + currentPosition,
              airbrakeSquareLength, airbrakeSquareLength),
        QRect(-airbrakeSquareLength + brakeZeroPosition + currentPosition, -airbrakeSquareLength / 2.0,
              airbrakeSquareLength, airbrakeSquareLength)
    };

    //All the rectangle positions are relative to the center of the circle because of translations when they are drawn
    QList<QRect> airbrakeRectanglesDesired = {
        QRect(-airbrakeSquareLength / 2.0, -brakeZeroPosition - desiredPosition,
              airbrakeSquareLength, airbrakeSquareLength),
        QRect(-brakeZeroPosition - desiredPosition, -airbrakeSquareLength / 2.0,
              airbrakeSquareLength, airbrakeSquareLength),
        QRect(-airbrakeSquareLength / 2.0, -airbrakeSquareLength + brakeZeroPosition + desiredPosition,
              airbrakeSquareLength, airbrakeSquareLength),
        QRect(-airbrakeSquareLength + brakeZeroPosition + desiredPosition, -airbrakeSquareLength / 2.0,
              airbrakeSquareLength, airbrakeSquareLength)
    };

    //Current airbrakes will be drawn on top of desired airbrakes, everything below here is drawn rotated and relative to circle position
    p->translate(circlePosition);
    p->rotate(45);

    //The rendering order of the desired and current airbrakes should change depending on which one is higher or lower
    if(m_latest->desiredAirbrakes - m_latest->currentAirbrakes >= AIRBRAKE_DESIRED_THRESHOLD) {
        p->setPen(rectLightPen);
        for(QRect rectangle : airbrakeRectanglesDesired) {
            p->fillRect(rectangle, m_highlightBrush);
        }
        p->setPen(rectPen);
        for(QRect rectangle : airbrakeRectanglesCurrent) {
            p->fillRect(rectangle, m_textBrush);
        }
    } else if(m_latest->desiredAirbrakes - m_latest->currentAirbrakes < -AIRBRAKE_DESIRED_THRESHOLD) {
        p->setPen(rectPen);
        for(QRect rectangle : airbrakeRectanglesCurrent) {
            p->fillRect(rectangle, m_textBrush);
        }
        p->setPen(rectLightPen);
        for(QRect rectangle : airbrakeRectanglesDesired) {
            p->fillRect(rectangle, m_highlightBrush);
        }
    } else { //Only current airbrakes should be rendered if the desired is within the threshold
        p->setPen(rectPen);
        for(QRect rectangle : airbrakeRectanglesCurrent) {
            p->fillRect(rectangle, m_textBrush);
        }
    }

    //Circle drawn on top of airbrakes
    p->setBrush(circleBrush);
    p->setPen(QPen(m_transparentBrush, 1));
    p->drawEllipse(QPointF(0, 0), circleRadius, circleRadius);

    //End rotation and translation of origin
    p->translate(QPoint(-circlePosition.x(), -circlePosition.y()));
    p->rotate(-45);
}
