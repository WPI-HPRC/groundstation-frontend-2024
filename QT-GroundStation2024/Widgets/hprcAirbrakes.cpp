#include "hprcAirbrakes.h"

#include "Windows/mainwindow.h"

#include "stylePainters.h"

#include <QApplication>

hprcAirbrakes::hprcAirbrakes(QWidget* parent) :
    hprcDisplayWidget(parent) {
    detailedViewEnabled = true;
    // subscribe to any data that needs to cause an update
    foreach(QWidget * w, qApp->topLevelWidgets())
        if (MainWindow* mainWin = qobject_cast<MainWindow*>(w))
        {
            connect(mainWin, SIGNAL(desiredAirbrakesUpdated(float)), this, SLOT(repaint()));
            connect(mainWin, SIGNAL(currentAirbrakesUpdated(float)), this, SLOT(repaint()));
        }


    m_widgetType = HPRC_AIRBRAKES;
}

void HPRCStyle::drawHPRCAirbrakes(QPainter* p, const hprcDisplayWidget* w) {
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

    std::string currentText = "Current: " + std::to_string((int)round(m_latest->currentAirbrakes * 100)) + "%";
    std::string desiredText = "Desired: " + std::to_string((int)round(m_latest->desiredAirbrakes * 100)) + "%";

    //    std::cout << currentText << std::endl;

    m_widgetLarge.setPointSize(w->width() / 20);
    p->setFont(m_widgetLarge);
    float currentTextHeight = QFontMetrics(p->font()).lineSpacing(); //Get height of current text
    float desiredTextHeight = QFontMetrics(p->font()).lineSpacing();

    float circleRadius = w->height() / 4;
    float airbrakeSquareLength = circleRadius /
        1.6; //The square length is the length such that the corners of all the squares are touching at the zero position (the 1.6 is just a guess)
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

    float brakeZeroPosition = sin(acos((airbrakeSquareLength / 2) / circleRadius)) *
        circleRadius; //Distance from center of circle where the squares should be when airbrakes are zero
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
    if (m_latest->desiredAirbrakes - m_latest->currentAirbrakes >= AIRBRAKE_DESIRED_THRESHOLD) {
        p->setPen(rectLightPen);
        for (QRect rectangle : airbrakeRectanglesDesired) {
            p->fillRect(rectangle, m_highlightBrush);
        }
        p->setPen(rectPen);
        for (QRect rectangle : airbrakeRectanglesCurrent) {
            p->fillRect(rectangle, m_textBrush);
        }
    }
    else if (m_latest->desiredAirbrakes - m_latest->currentAirbrakes < -AIRBRAKE_DESIRED_THRESHOLD) {
        p->setPen(rectPen);
        for (QRect rectangle : airbrakeRectanglesCurrent) {
            p->fillRect(rectangle, m_textBrush);
        }
        p->setPen(rectLightPen);
        for (QRect rectangle : airbrakeRectanglesDesired) {
            p->fillRect(rectangle, m_highlightBrush);
        }
    }
    else { //Only current airbrakes should be rendered if the desired is within the threshold
        p->setPen(rectPen);
        for (QRect rectangle : airbrakeRectanglesCurrent) {
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