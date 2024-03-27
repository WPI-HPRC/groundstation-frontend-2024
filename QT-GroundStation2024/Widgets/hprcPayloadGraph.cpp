#include "hprcPayloadGraph.h"

#include "stylepainters.h"

#include <QMouseEvent>
#include <QApplication>

hprcPayloadGraph::hprcPayloadGraph(QWidget* parent) :
    hprcDisplayWidget(parent)
{
    setMouseTracking(true);
    m_widgetType = HPRC_PAYLOAD_GRAPH;
    foreach(QWidget * w, qApp->topLevelWidgets())
        if (MainWindow* mainWin = qobject_cast<MainWindow*>(w))
        {
            mainWindow = mainWin;
            connect(mainWin, SIGNAL(tick()), this, SLOT(repaint()));
            connect(mainWin, SIGNAL(accUpdated(int)), this, SLOT(updateVerticalSpeed()));
        }
}

void hprcPayloadGraph::updateVerticalSpeed() {
    if (mainWindow->m_currentData.altData1.length() > 1) {
        verticalSpeedData.append(MainWindow::graphPoint{ (mainWindow->m_currentData.altData1[mainWindow->m_currentData.altData1.length() - 1].value - mainWindow->m_currentData.altData1[mainWindow->m_currentData.altData1.length() - 2].value) / ((mainWindow->m_currentData.altData1[mainWindow->m_currentData.altData1.length() - 1].time - mainWindow->m_currentData.altData1[mainWindow->m_currentData.altData1.length() - 2].time) / 1000), mainWindow->m_currentData.altData1[mainWindow->m_currentData.altData1.length() - 1].time });
    }
}

void hprcPayloadGraph::mouseMoveEvent(QMouseEvent* e)
{
    m_mousePos = e->pos();

}

void HPRCStyle::drawHPRCPayloadGraph(QPainter* p, const hprcDisplayWidget* w) {
    hprcPayloadGraph* widget = (hprcPayloadGraph*)w;

    p->setRenderHint(QPainter::Antialiasing);
    QPen textPen(m_textBrush, 3);

    /*UNCOMMENT SECTION BELOW WHEN NOT DEBUG
    if(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - ((hprcPayloadGraph*) w)->lastTime).count() >= 1) {
        ((hprcPayloadGraph*) w)->fps = (int) ((hprcPayloadGraph*) w)->frames;
        ((hprcPayloadGraph*) w)->lastTime = std::chrono::high_resolution_clock::now();
        ((hprcPayloadGraph*) w)->frames = 0;
    }
    ((hprcPayloadGraph*) w)->frames ++;
    p->setPen(textPen);
    m_widgetLarge.setPointSize(20);
    p->setFont(m_widgetLarge);
    p->drawText(QRect(50, 30, 500, 500),
                    Qt::AlignHCenter, QString::fromStdString("FPS: " + std::to_string(((hprcPayloadGraph*) w)->fps)));
    */

    int width = w->rect().width();
    int height = w->rect().height();

    double scaleF = 0.75;
    double paddingRatio = (1 - scaleF) / 2.0;

    int margin = fmin(paddingRatio * width, paddingRatio * height);

    //QRectF drawBox = w->rect().adjusted(margin, margin, -margin, -margin);

    QRectF drawBox = w->rect();
    drawBox.adjust(margin, 0, 0, -margin / 4);

    // label padding = 7.5%
    //int lMargin = drawBox.height() * 0.075;
    //drawBox.adjust(lMargin, 0, 0, -lMargin);

    float fontSize = drawBox.height() / 28;

    QPointF topRightTop = QPointF(drawBox.topRight().x(), drawBox.topRight().y() + fontSize * 2);
    QPointF bottomLeftBottom = drawBox.bottomLeft();

    QPointF bottomLeftTop(drawBox.left(), drawBox.y() + drawBox.height() / 2);
    QPointF topRightBottom(drawBox.right(), drawBox.y() + drawBox.height() / 2);

    QRectF top(topRightTop, bottomLeftTop);

    //QRectF middle(topRightMiddle, bottomLeftMiddle);
    QRectF bottom(topRightBottom, bottomLeftBottom);
    bottom.adjust(0, fontSize * 2, 0, 0); //Font size needs to be multiplied by 2 for some reason

    double range = 1000 * 200;
    double start = m_latest->altData1.length() > 0 ? m_latest->altData1[0].time : 0;
    if (m_latest->altData1.length() > 1) {
        double averageTimePerAltitude = 0;
        //Get average descent rate for past 10 seconds
        int initialDataPoint = m_latest->altData1.length() - 1;
        //Find index of data point from about 10 seconds ago
        while (initialDataPoint > 0 && m_latest->rocketTime - m_latest->altData1[initialDataPoint].time < 10 * 1000) {
            initialDataPoint--;
        }
        //Negative to counteract the fact that altitude decreases over time
        averageTimePerAltitude = -(m_latest->altData1[m_latest->altData1.length() - 1].time -
            m_latest->altData1[initialDataPoint].time) /
            (m_latest->altData1[m_latest->altData1.length() - 1].value -
                m_latest->altData1[initialDataPoint].value);

        //The range of the graph is calculated based on how long it would take to reach 0 altitude at the current descent rate
        //current time + (time / altitude) * (current altitude (amount of altitude required to reach 0 altitude))
        range = m_latest->altData1[m_latest->altData1.length() - 1].time - m_latest->altData1[0].time +
            averageTimePerAltitude * m_latest->altData1[m_latest->altData1.length() - 1].value;
        //std::cout << "Range: " << range << std::endl;
    }
    //range = 1000 * 100;
    //std::cout << "length: " << widget->altitudeData.length() << std::endl;

    bool drawT = false;
    if (drawBox.contains(w->m_mousePos)) {
        drawT = true;
    }

    // <---- draw ----> //

    p->setBrush(m_backgroundBrush);
    p->drawRect(drawBox);

    //Draw label text
    std::string altitudeText;
    std::string descentRateText;
    if (m_latest->state == MainWindow::RocketState::DROGUE_DESCENT &&
        m_latest->state == MainWindow::RocketState::MAIN_DESCENT) {
        altitudeText = "Altitude: " + std::to_string((int)round(m_latest->p_altitude)) + " [m]";
        descentRateText = "Vertical Speed: " + std::to_string((int)round(m_latest->p_velZ)) + " [m/s]";
    }
    else {
        altitudeText = "Altitude: X [m]";
        descentRateText = "Vertical Speed: X [m/s]";
    }
    p->setPen(textPen);
    m_widgetLarge.setPointSize(fontSize);
    p->setFont(m_widgetLarge);
    //std::cout << "draw box: x: " << drawBox.x() << ", y: " << drawBox.y() << ", width: " << drawBox.width() << ", height: " << drawBox.height() << std::endl;
    p->drawText(QRect(top.x(), top.y() - fontSize * 2,
        top.width(), top.height()),
        Qt::AlignHCenter, QString::fromStdString(altitudeText)); //QString::fromStdString(altitudeText)
    p->drawText(QRect(bottom.x(), bottom.y() - fontSize * 2,
        bottom.width(), bottom.height()),
        Qt::AlignHCenter, QString::fromStdString(descentRateText));

    //Debug thing below
    //if(m_latest->altData.length() == 0) {
    //    m_latest->altData.append(MainWindow::graphPoint{(float) m_latest->altData.length() + 100, (float) start + 1000}); //(float) (m_latest->rocketTime - range)
    //}

    if (m_latest->state == MainWindow::RocketState::DROGUE_DESCENT &&
        m_latest->state == MainWindow::RocketState::MAIN_DESCENT) {
        Range altitudeSubGraphRange = drawHPRCSubGraph(p, top, QColor("#00FF00"), &m_latest->altData1, range, start, w,
            drawT, 0,
            m_latest->altData1.length() > 0 ? m_latest->altData1[0].value
            : 1300, false, false,
            &((hprcPayloadGraph*)w)->startIndexAltitude,
            &((hprcPayloadGraph*)w)->graphPolygonAltitude);
        Range yRange = getDataYRange(&((hprcPayloadGraph*)w)->verticalSpeedData);
        drawHPRCSubGraph(p, bottom, m_highlightBrush.color(), &((hprcPayloadGraph*)w)->verticalSpeedData, range,
            start, w, drawT, yRange.min, yRange.max, false, false,
            &((hprcPayloadGraph*)w)->startIndexVerticalSpeed,
            &((hprcPayloadGraph*)w)->graphPolygonVerticalSpeed);
        //drawHPRCSubGraph(p, bottom, QColor("#FFFF00"), widget->altitudeData, range, start, w, drawT);

        //Draw a fun parachute graphic for the altitude graph
        if (m_latest->altData1.length() > 0) {
            float lastGraphX =
                -(m_latest->altData1[m_latest->altData1.length() - 1].time - start) / (range) * (top.width()) +
                top.right();
            double subGraphScale = fmax(1.0, altitudeSubGraphRange.max - altitudeSubGraphRange.min);
            float lastGraphY =
                -((m_latest->altData1[m_latest->altData1.length() - 1].value - altitudeSubGraphRange.min) /
                    subGraphScale) * (top.height() * 0.97) + top.bottom();
            float boxWidth = drawBox.height() / 24.0;
            float boxHeight = drawBox.height() / 24.0;
            QRect parachuteRect(lastGraphX - boxWidth / 2.0, lastGraphY - boxHeight / 2.0, boxWidth, boxHeight);

            QBrush parachuteBrush = QBrush(QColor(255, 255, 255, 128));
            QPen transparentPen = QPen(QColor(255, 255, 255, 0));
            QPen linePen = QPen(QColor(255, 255, 255, 128));
            p->setBrush(parachuteBrush);
            p->setPen(transparentPen);
            p->drawRect(parachuteRect);

            QPainterPath semiCirclePath;
            float parachuteX = lastGraphX - boxWidth;
            float parachuteY = lastGraphY - boxWidth * 1.5;
            semiCirclePath.moveTo(parachuteX, parachuteY);
            semiCirclePath.arcTo(parachuteX, parachuteY - boxHeight, boxWidth * 2.0, boxHeight * 2.0, 0, 180);

            p->drawPath(semiCirclePath);

            p->setPen(linePen);
            p->drawLine(QLine(lastGraphX - boxWidth / 2.0, lastGraphY - boxHeight / 2.0, lastGraphX - boxWidth,
                lastGraphY - boxHeight * 1.5));
            p->drawLine(QLine(lastGraphX + boxWidth / 2.0, lastGraphY - boxHeight / 2.0, lastGraphX + boxWidth,
                lastGraphY - boxHeight * 1.5));
        }

        p->setBrush(m_transparentBrush);
        p->setPen(textPen);
        p->drawLine(top.topRight(), bottom.bottomRight());
        p->drawLine(bottom.bottomRight(), bottom.bottomLeft());
    }
}

HPRCStyle::Range HPRCStyle::getDataYRange(QList<MainWindow::graphPoint>* data) {
    double scaleMax = 1;
    double scaleMin = 999999;
    for (int i = 0; i < data->size(); i++) {
        ////        if(!(data.at(i).time < (start - range)))
        ////        {
        if (data->at(i).value > scaleMax) {
            scaleMax = data->at(i).value;
        }
        if (data->at(i).value < scaleMin) {
            scaleMin = data->at(i).value;
        }

        ////        }
    }

    return Range{ scaleMin, scaleMax };
}