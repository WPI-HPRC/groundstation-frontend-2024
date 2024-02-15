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
