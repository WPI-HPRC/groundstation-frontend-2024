#include "hprcServoStatusWidget.h"

#include "stylepainters.h"

#include "Windows/mainwindow.h"

#include <QApplication>

hprcServoStatusWidget::hprcServoStatusWidget(QWidget* parent) {
    // subscribe to any data that needs to cause an update
    foreach(QWidget * w, qApp->topLevelWidgets())
        if (MainWindow* mainWin = qobject_cast<MainWindow*>(w))
        {
            connect(mainWin, SIGNAL(payloadServo1PositionUpdated(float)), this, SLOT(repaint()));
            connect(mainWin, SIGNAL(desiredPayloadServo1PositionUpdated(float)), this, SLOT(repaint()));
        }
    m_widgetType = HPRC_SERVO_STATUS;
}

void HPRCStyle::drawServoStatusServo(QPainter* p, const hprcDisplayWidget* w, QString title, int position,
    int desiredPosition, float x, float yOffset, float width) {
    //Draw a happy servo picture
    p->setPen(QPen(m_textBrush, w->rect().width() / 200));
    p->setBrush(m_transparentBrush);
    float servoRectWidth = w->rect().width() / 5;
    float servoRectHeight = w->rect().width() / 8;
    float servoRect2Width = servoRectHeight / 4;
    float yMargin = servoRectHeight * 2;
    QRect servoRect = QRect(x - (servoRectWidth + servoRect2Width) / 2, w->rect().bottom() - yMargin - yOffset * servoRectHeight, servoRectWidth,
        servoRectHeight);
    p->drawRect(servoRect);
    p->drawRect(QRect(servoRect.x() + servoRect.width() / 4, servoRect.top(), servoRect.width() / 12,
        -servoRect.width() / 4));

    float textMargin = w->rect().width() / 50;

    //Draw title
    m_widgetLarge.setPointSize(servoRect.height() / 4);
    p->setFont(m_widgetLarge);
    p->drawText(servoRect,
        Qt::AlignCenter, title);

    //Draw encoder value
    QString valueString = QString::number(position);
    QString desiredValueString = QString::number(desiredPosition);

    float textSize = w->width() / 30.0;
    m_widgetLarge.setPointSize(textSize);
    p->setFont(m_widgetLarge);
    p->drawText(QRect(x - width / 2, servoRect.y() + servoRect.height() + textMargin,
        width, 100),
        Qt::AlignHCenter, "Measured Position: " + valueString);
    p->drawText(QRect(x - width / 2, servoRect.y() + servoRect.height() + textMargin + textSize + textMargin,
        width, 100),
        Qt::AlignHCenter, "Desired Position: " + desiredValueString);


    //Draw position indicator
    QRect measuredPositionRect = QRect();
    QRect desiredPositionRect = QRect();
}

void HPRCStyle::drawHprcServoStatus(QPainter* p, const hprcDisplayWidget* w) {
    p->setRenderHint(QPainter::Antialiasing);

    drawServoStatusServo(p, w, "Servo 1", m_latest->p_actualServoPos1, m_latest->p_desiredServoPos1,
        (w->rect().center().x() - w->rect().x()) / 2, 4, w->rect().center().x() - w->rect().x());
    drawServoStatusServo(p, w, "Servo 2", m_latest->p_actualServoPos2, m_latest->p_desiredServoPos2,
        w->rect().center().x() + (w->rect().center().x() - w->rect().x()) / 2, 4,
        w->rect().center().x() - w->rect().x());
    drawServoStatusServo(p, w, "Servo 3", m_latest->p_actualServoPos3, m_latest->p_desiredServoPos3,
        (w->rect().center().x() - w->rect().x()) / 2, 0, w->rect().center().x() - w->rect().x());
    drawServoStatusServo(p, w, "Servo 4", m_latest->p_actualServoPos4, m_latest->p_desiredServoPos4,
        w->rect().center().x() + (w->rect().center().x() - w->rect().x()) / 2, 0,
        w->rect().center().x() - w->rect().x());
}