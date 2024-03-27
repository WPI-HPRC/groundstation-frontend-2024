#include "hprcAttitudeWidget.h"

#include "stylepainters.h"

#include "Windows/mainwindow.h"
#include <QMouseEvent>
#include <QApplication>

hprcAttitudeWidget::hprcAttitudeWidget(QWidget* parent) :
    hprcDisplayWidget(parent)
{
    //    setMouseTracking(true);
    m_widgetType = HPRC_Attitude;
    foreach(QWidget * w, qApp->topLevelWidgets())
        if (MainWindow* mainWin = qobject_cast<MainWindow*>(w))
        {
            connect(mainWin, SIGNAL(orientationUpdated()), this, SLOT(repaint()));
            connect(mainWin, SIGNAL(gyroUpdated()), this, SLOT(repaint()));
        }
}

void hprcAttitudeWidget::mouseMoveEvent(QMouseEvent* e)
{
    m_mousePos = e->pos();
}

void HPRCStyle::drawHPRCAttitudeWidget(QPainter* p, const hprcDisplayWidget* w) {

    if (w->width() < 100)
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

    boundingBox.adjust(boundingBox.width() / 2 - sizeMin / 2,
        boundingBox.height() / 2 - sizeMin / 2,
        boundingBox.width() / 2 - sizeMin / 2,
        boundingBox.height() / 2 - sizeMin / 2
    );

    boundingBox.setHeight(sizeMin);
    boundingBox.setWidth(sizeMin);

    // For testing purposes

    float crossWidth = sizeMin / 10;
    crossWidth /= 2;

    bgPen.setWidth(crossWidth);
    // -- Get the angles and use them --

    float pitch, yaw, roll;

    pitch = qRadiansToDegrees(atan2(2 * (m_latest->w * m_latest->i + m_latest->j * m_latest->k),
        1 - 2 * (pow(m_latest->i, 2) + pow(m_latest->j, 2))));

    yaw = qRadiansToDegrees(
        -M_PI / 2 + 2 * atan2(sqrt(1 + 2 * (m_latest->w * m_latest->j - m_latest->i * m_latest->k)),
            sqrt(1 - 2 * (m_latest->w * m_latest->j - m_latest->i * m_latest->k))));

    roll = qRadiansToDegrees(atan2(2 * (m_latest->w * m_latest->k + m_latest->i * m_latest->j),
        1 - 2 * (pow(m_latest->j, 2) + pow(m_latest->k, 2))));

    if (w->objectName() == QString("p_attitude"))
    {
        qDebug("Attitude");
        pitch = qRadiansToDegrees(atan2(2 * (m_latest->p_w * m_latest->p_i + m_latest->p_j * m_latest->p_k),
            1 - 2 * (pow(m_latest->p_i, 2) + pow(m_latest->p_j, 2))));

        yaw = qRadiansToDegrees(
            -M_PI / 2 + 2 * atan2(sqrt(1 + 2 * (m_latest->p_w * m_latest->p_j - m_latest->p_i * m_latest->p_k)),
                sqrt(1 - 2 * (m_latest->p_w * m_latest->p_j - m_latest->p_i * m_latest->p_k))));

        roll = qRadiansToDegrees(atan2(2 * (m_latest->p_w * m_latest->p_k + m_latest->p_i * m_latest->p_j),
            1 - 2 * (pow(m_latest->p_j, 2) + pow(m_latest->p_k, 2))));
    }
    else
    {
        qDebug("Not attitude");
    }



    pitch += 180;

    if (pitch > 180)
        pitch -= 360;

    //    float roll = m_latest->gyroZ;

        // Clamp to values
    pitch = fminf(m_AttitudeDegreeOffsetPitch + m_AttitudeMaxDegreeRange,
        fmaxf(m_AttitudeDegreeOffsetPitch - m_AttitudeMaxDegreeRange, pitch));
    yaw = fminf(m_AttitudeDegreeOffsetYaw + m_AttitudeMaxDegreeRange,
        fmaxf(m_AttitudeDegreeOffsetYaw - m_AttitudeMaxDegreeRange, yaw));

    // Normalize between -1 -> +1
    float pitchNormalized = (pitch - m_AttitudeDegreeOffsetPitch) / m_AttitudeMaxDegreeRange;
    float yawNormalized = (yaw - m_AttitudeDegreeOffsetYaw) / m_AttitudeMaxDegreeRange;

    float yawY = boundingBox.center().y() + (boundingBox.height() / 2 - crossWidth * 2) * -1 * pitchNormalized;
    float pitchX = boundingBox.center().x() + (boundingBox.width() / 2 - crossWidth * 2) * yawNormalized;


    // -- Draw the info --

    // Center dot

    p->setPen(QPen(m_textBrush, 2));

    float circleLocation;

    std::vector<float> vec = m_AttitudeCircleLocationsDegrees[m_latest->state];

    if (vec.empty()) {
        qDebug() << "Couldn't draw attitude widget for state" << m_latest->state;
        return;
    }

    bool rocketIsWithinGraph = pitch * pitch + yaw * yaw < vec.back() * vec.back();

    std::reverse(vec.begin(), vec.end());

    QColor bgCol = bgPen.color();
    bgCol.setAlphaF(0.5);

    QPen outline = QPen(QBrush(bgCol), 4);

    QColor fgCol = m_highlightBrush.color();

    for (float circleLocationDegrees : vec) {

        circleLocation = circleLocationDegrees / (m_AttitudeMaxDegreeRange);

        float dist = hypot(yaw, pitch) - circleLocationDegrees;

        p->setPen(outline);

        if (rocketIsWithinGraph) {
            fgCol.setAlphaF(fminf(
                powf(1 - fabsf(dist) / m_AttitudeMaxDegreeRange, 4),
                powf((dist + circleLocationDegrees) / m_AttitudeMaxDegreeRange, 3)));
        }
        else {
            fgCol.setAlphaF(1);
        }

        p->setBrush(QBrush(fgCol));


        p->drawEllipse(roundf(boundingBox.center().x() - boundingBox.width() / 2 * circleLocation),
            roundf(boundingBox.center().y() - boundingBox.width() / 2 * circleLocation),
            roundf(boundingBox.width() * circleLocation), roundf(boundingBox.width() * circleLocation));
    }

    // Crosshair
    p->setOpacity(1);
    p->setPen(QPen(m_textBrush, 2));
    QPoint center((int)pitchX, (int)yawY);

    p->drawLine(center.x() - 4, center.y(), center.x() + 4, center.y());
    p->drawLine(center.x(), center.y() - 4, center.x(), center.y() + 4);

    if (rocketIsWithinGraph)
        p->setPen(QPen(m_textBrush, 2));
    else
        p->setPen(QPen(m_highlightBrush, 2));

    m_widgetLarge.setPointSize(crossWidth * 3);

    p->setFont(m_widgetLarge);

    m_widgetLarge.setPointSize(crossWidth * 2);


    p->setPen(QPen(m_textBrush, 2));
    p->setFont(m_widgetLarge);

    p->drawText(QRect(boundingBox.x() - 20 - crossWidth * 3,
        boundingBox.y() - 30,
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
        QString::asprintf("Yaw: %.0lf", roll)); // TODO: change this to the actual value

    m_widgetLarge.setPointSize(crossWidth * 2);

    p->setFont(m_widgetLarge);

    if (rocketIsWithinGraph)
        p->setPen(QPen(m_textBrush, 2));
    else
        p->setPen(QPen(m_highlightBrush, 2));

    p->drawText(QRect(boundingBox.x() - 20 - crossWidth * 3,
        boundingBox.y() - 30 + crossWidth * 3,
        crossWidth * 6,
        crossWidth * 3
    ),
        Qt::AlignCenter,
        QString::asprintf("%.0lfº", pitch));


    p->setPen(QPen(m_textBrush, 2));

    p->drawText(QRect(boundingBox.x() + boundingBox.width() + 20 - crossWidth * 5,
        boundingBox.y() - 30,
        crossWidth * 10,
        crossWidth * 3
    ),
        Qt::AlignCenter,
        "ROLL");

    if (rocketIsWithinGraph)
        p->setPen(QPen(m_textBrush, 2));
    else
        p->setPen(QPen(m_highlightBrush, 2));
    p->drawText(QRect(boundingBox.x() + boundingBox.width() + 20 - crossWidth * 3,
        boundingBox.y() - 30 + crossWidth * 3,
        crossWidth * 6,
        crossWidth * 3
    ),
        Qt::AlignCenter,
        QString::asprintf("%.0lfº", yaw));

}