#include "stylepainters.h"
#include <QApplication>
#include <QStyle>
#include <QPalette>
#include <QStyleOption>
#include <iomanip>
#include <iostream>
#include <QFontDatabase>
#include "mainwindow.h"
#include <QDateTime>

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
    m_widgetLarge = QFont(overpassMono, 20, 5, false);
    m_widgetMedium = QFont(overpassMono, 15, 5, false);

    double ticks[5] = {1, 0.85, 0.5, 0.25, 0}; // these need to be ordered bottom to top.  the system draws from top-left so "1" is the bottom (top - 1 * height)
    QString states[4] = {"BOOST", "CRUISE", "DROGUE", "MAIN"}; // will be associated with the space between [n] and [n+1] eg BOOST is between 1 and 0.85
    for(int index = 0; index < (sizeof(ticks)/sizeof(ticks[0])) - 1; index++)
    {
        double start = ticks[index];
        double end = ticks[index + 1];
        double middle = (start - end) / 2.0 + end;
        m_stateMap.insert(std::make_pair(start, QString("-")));
        m_stateMap.insert(std::make_pair(middle, states[index]));

    }
    m_stateMap.insert(std::make_pair(ticks[sizeof(ticks)/sizeof(ticks[0]) - 1], QString("-")));

    m_alarmMapA.insert(std::make_pair(HPRCStyle::ALARM_MasterAbort, true));
    m_alarmMapA.insert(std::make_pair(HPRCStyle::ALARM_Pitch, true));

    m_alarmMapW.insert(std::make_pair(HPRCStyle::ALARM_MasterWarn, true));
    m_alarmMapW.insert(std::make_pair(HPRCStyle::ALARM_PowerLoss, false));
    m_alarmMapW.insert(std::make_pair(HPRCStyle::ALARM_LowPower, false));
    m_alarmMapW.insert(std::make_pair(HPRCStyle::ALARM_SignalLoss, true));
    m_alarmMapW.insert(std::make_pair(HPRCStyle::ALARM_EarlyChute, true));
    m_alarmMapW.insert(std::make_pair(HPRCStyle::ALARM_Ballistic, false));
    m_alarmMapW.insert(std::make_pair(HPRCStyle::ALARM_MainDeployFail, false));


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
    p->setPen(QPen(m_transparentBrush, 0));
    p->drawRoundedRect(QRectF(o->rect).adjusted(0.5, 0.5, -0.5, -0.5), 10, 10);

}

void HPRCStyle::drawHPRCTimeline(QPainter *p, const hprcDisplayWidget *w)
{

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
    QPoint bottomRight(drawX + (0.9 - scaleF) * drawWidth, drawBox.bottom());
    QPoint topLeftSlot(drawX + (0.9 - scaleF) * drawWidth, drawY);
    QPoint topLeftFill(drawX + (0.9 - scaleF) * drawWidth, drawY + drawHeight - (drawHeight * (w->m_filledPercent / 100.0)));

    QRect timelineSlot(topLeftSlot, bottomRight);
    QRect timelineFill(topLeftFill, bottomRight);

    // <------------------- draw --------------------->//

    p->setPen(bgPen);
    p->drawRoundedRect(timelineSlot, (scaleF*drawWidth/2), (scaleF*drawWidth/2));
    p->setPen(fgPen);
    if(w->m_filledPercent > 1)
        p->drawRoundedRect(timelineFill, (scaleF*drawWidth/2), (scaleF*drawWidth/2));
    p->setPen(tickPen);

    for(const auto& [position, label] : m_stateMap)
    {
        p->setFont(m_widgetLarge);
        QTransform rPt(1, 0, 0, 1, topLeftSlot.x() - (p->font().pointSize()) * label.length(), position * drawHeight + drawY + p->font().pointSize() * 0.3);
        p->setTransform(rPt);
        p->drawText(0, 0, label);

        if(label.startsWith("-"))
        {
            p->setPen(bgPen);
            p->setFont(m_widgetMedium);
            p->drawText(p->font().pointSize() * -8 + 20, 0, QString("00:00:00"));
            p->setFont(m_widgetLarge);
            p->setPen(tickPen);
        }
    }


}

void HPRCStyle::drawHPRCGauge(QPainter *p, const hprcDisplayWidget *w)
{
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

    QPen fgPen(m_highlightBrush, 5);

    fgPen.setCapStyle(Qt::RoundCap);

    bgPen.setWidth(sizeMin/10);
    fgPen.setWidth(sizeMin/10 -5);


    m_widgetLarge.setPointSize(sizeMin/13);
    p->setFont(m_widgetLarge);

    // <---- draw ----> //

    p->setPen(bgPen);
    p->drawArc(boundingBox, -extraArc*16, (180 + 2 * extraArc)*16);
    p->setPen(fgPen);
    p->setPen(textPen);
    p->drawText(boundingBox.adjusted(0, 30, 0, 30), Qt::AlignCenter, w->m_label);
//    p->drawRect(w->rect());

}

void HPRCStyle::drawHPRCGaugeData(QPainter *p, const hprcDisplayWidget *w)
{
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

    QPen fgPen(m_highlightBrush, 5);

    fgPen.setCapStyle(Qt::RoundCap);
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



    // <---- draw ----> //

    p->setPen(fgPen);
    p->drawArc(boundingBox, (180 * 16 + extraArc*16), (180 + 2 * extraArc)*-16 * (fmax(0, fmin(w->m_filledPercent/w->m_max, 1))));
    p->setPen(textPen);

    m_widgetLarge.setPointSize(sizeMin/7);
    p->setFont(m_widgetLarge);

    p->drawText(boundingBox.adjusted(0, -30, 0, -30), Qt::AlignCenter, dataString);
//    p->drawRect(w->rect());

}

void HPRCStyle::drawHPRCGraph(QPainter *p, const hprcDisplayWidget *w)
{


    p->setRenderHint(QPainter::Antialiasing);
    QPen textPen(m_textBrush, 3);

    int width= w->rect().width();
    int height = w->rect().height();

    double scaleF = 0.75;
    double paddingRatio = (1-scaleF) / 2.0;

    int margin = fmin(paddingRatio * width, paddingRatio * height);

    QRectF drawBox = w->rect().adjusted(margin, margin, -margin, -margin);

    // label padding = 7.5%
    int lMargin = drawBox.height() * 0.075;
    drawBox.adjust(lMargin, 0, 0, -lMargin);

    QPointF topRightTop = drawBox.topRight();
    QPointF bottomLeftBottom = drawBox.bottomLeft();

    int row2Top = drawBox.y() + drawBox.height()/3;
    int row3Top = drawBox.y() + 2 * drawBox.height()/3;

    QPointF bottomLeftTop(drawBox.left(), row2Top);
    QPointF topRightMiddle(drawBox.right(), row2Top);
    QPointF bottomLeftMiddle(drawBox.left(), row3Top);
    QPointF topRightBottom(drawBox.right(), row3Top);

    QRectF top(topRightTop, bottomLeftTop);
    QRectF middle(topRightMiddle, bottomLeftMiddle);
    QRectF bottom(topRightBottom, bottomLeftBottom);

    double range = 5000;
    double start = m_latest->rocketTime - range;

    bool drawT = false;
    if(drawBox.contains(w->m_mousePos))
    {
        drawT = true;
    }

    // <---- draw ----> //

    p->setBrush(m_backgroundBrush);
    p->drawRect(drawBox);

    drawHPRCSubGraph(p, top, m_highlightBrush.color(), m_latest->accData, range, start, w, drawT);
    drawHPRCSubGraph(p, middle, QColor("#2c4985"), m_latest->velData, range, start, w, drawT);
    drawHPRCSubGraph(p, bottom, QColor("#471d57"), m_latest->altData, range, start, w, drawT);



    p->setBrush(m_transparentBrush);
    p->setPen(textPen);
    p->drawLine(top.topRight(), bottom.bottomRight());
    p->drawLine(bottom.bottomRight(), bottom.bottomLeft());

}

void HPRCStyle::drawHPRCSubGraph(QPainter *p, QRectF rect, QColor bg, QList<MainWindow::graphPoint> data, double range, double start, const hprcDisplayWidget *w, bool drawTooltip)
{

    QBrush lightHighlighterBrush(QColor(255, 255, 255, 64));

    QPointF bottomPt = rect.bottomLeft();
//    bottomPt.setY(rect.bottom() + rect.height()/3);
    QLinearGradient gradient(bottomPt, rect.topLeft());
//    gradient.setColorAt(0, m_transparentBrush.color());
//    gradient.setColorAt(1, bg);
    gradient.setColorAt(0, bg);
    gradient.setColorAt(1, m_transparentBrush.color());

//    p->setPen(QPen(m_transparentBrush, 3));
    p->setPen(QPen(bg, 3));
    QList<QPointF> pointsToDraw;
    double max = 9999;
    double scaleMax = 1;
    double scaleMin = 999999;
    for(int i = 0; i < data.size(); i++)
    {
////        if(!(data.at(i).time < (start - range)))
////        {
            if(data.at(i).value > scaleMax)
            {
                scaleMax = data.at(i).value;
            }
            if(data.at(i).value < scaleMin)
            {
                scaleMin = data.at(i).value;
            }

////        }
    }

    double scale = fmax(1.0, scaleMax - scaleMin);

    QRectF ptHighlight(-100,0,0,0);
    QPointF highlighted(-100,0);
    QString ptLabel("");

    for(MainWindow::graphPoint g : data)
    {
        double valX = 0;
        double valY = 0;
        if(valY < max)
            max = valY;
        valX = -(g.time - start) / (range) * (rect.width()) + rect.right();
        valY = -((g.value - scaleMin) / scale) * (rect.height() * 0.97) + rect.bottom();

        if(valX - w->m_mousePos.x() > (rect.width() / 2.0 / data.size()) && valX - w->m_mousePos.x() < -(rect.width() / 2.0 / data.size()))
        {
            ptHighlight = QRectF(valX - 25, rect.top(), 50, rect.height());
            ptLabel = QString::number(g.value);
            highlighted = QPointF(valX, valY);
        } else {

        }

        pointsToDraw.append(QPointF(valX, valY));

    }





    gradient.setFinalStop(rect.topLeft());
    pointsToDraw.append(rect.bottomLeft());
    pointsToDraw.append(rect.bottomRight());
    p->setBrush(QBrush(gradient));
//    p->setBrush(m_transparentBrush);
    p->drawPolygon(QPolygonF(pointsToDraw));
    p->setPen(QPen(m_highlightBrush, 3));
    p->setBrush(lightHighlighterBrush);

    if(drawTooltip)
    {
        p->drawRect(ptHighlight);
        p->drawLine(highlighted.x(), rect.top(), highlighted.x(), rect.bottom());




        p->setBrush(m_highlightBrush);
        p->drawEllipse(highlighted, 5, 5);

        p->setPen(QPen(m_textBrush, 3));
        p->setFont(m_widgetMedium);
        p->drawText(ptHighlight, ptLabel);
        p->setPen(QPen(m_highlightBrush, 3));
    }


}

void HPRCStyle::drawHPRCAlarmPanel(QPainter *p, const hprcDisplayWidget *w)
{
    p->setRenderHint(QPainter::Antialiasing);
    p->setBrush(m_transparentBrush);
    QPen linePen(m_backgroundBrush, 4);
    linePen.setCapStyle(Qt::RoundCap);
    p->setPen(linePen);


    int abortConditions = m_alarmMapA.size();
    int warnConditions = m_alarmMapW.size();

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

    for(const auto& [type, active] : m_alarmMapA)
    {
        int y = w->rect().y() + hPadding + (height * i / total);
        drawHPRCAlarmFromEnum(p, x, y, size, type, active, startX, startYA);

        i++;
    }
    i++;
    p->setPen(linePen);
    int startYW = w->rect().y() + hPadding + (height * i / total + size * 0.5);
    p->drawLine(startX, startYW, startX, startYW + (size * (warnConditions - 1)));


    for(const auto& [type, active] : m_alarmMapW)
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

