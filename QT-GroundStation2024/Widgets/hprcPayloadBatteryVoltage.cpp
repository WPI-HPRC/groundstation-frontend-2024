#include "hprcPayloadBatteryVoltage.h"

#include "stylepainters.h"

hprcPayloadBatteryVoltage::hprcPayloadBatteryVoltage(QWidget* parent) :
    hprcDisplayWidget(parent)
{
    m_widgetType = HPRC_PayloadBatteryVoltage;
}

void HPRCStyle::drawHPRCPayloadBatteryVoltage(QPainter* p, const hprcDisplayWidget* w) {
    const QRect boundingBox = w->rect();

    p->setPen(QPen(m_textBrush, boundingBox.width() / 100));
    p->setFont(m_widgetLarge);

    // Draw a title for the widget
    p->drawText(boundingBox,
        Qt::AlignHCenter,
        "Battery Voltage");

    //Draw battery visual
    const float batteryWidth = boundingBox.width() / 4;
    const float batteryHeight = boundingBox.height() / 2;
    QList<QRect> batteryRects;
    batteryRects.append(QRect(w->rect().width() / 2.0 - batteryWidth / 2, w->rect().height() / 2.0 - batteryHeight / 2,
        batteryWidth, batteryHeight));
    QRect* batteryRect = &batteryRects[0];
    const float leadWidth = batteryRect->width() / 4.0;
    const float leadHeight = batteryRect->height() / 8.0;
    batteryRects.append(
        QRect(batteryRect->x() + batteryWidth / 2 - leadWidth / 2, batteryRect->y() - leadHeight, leadWidth,
            leadHeight));

    float batteryVoltage = m_latest->payloadBatteryVoltage;

    // Draw the current voltage
    QString valueString = QString::number(batteryVoltage, 'f', 2);
    //Make sure there are no trailing zeros
    while (valueString.length() > 0 && valueString.back() == '0') {
        valueString.remove(valueString.length() - 1, 1);
    }
    if (valueString.back() == '.') {
        valueString.remove(valueString.length() - 1, 1);
    }

    hprcPayloadBatteryVoltage* batteryWidget = (hprcPayloadBatteryVoltage*)p;
    QBrush batteryBrush =
        batteryVoltage > batteryWidget->LOW_THRESHOLD ? QBrush(QColor(0, 255, 0)) : QBrush(QColor(255, 0, 0));
    float batteryCharge = std::min(std::max((batteryVoltage - hprcPayloadBatteryVoltage::MIN_VOLTAGE) /
        (hprcPayloadBatteryVoltage::MAX_VOLTAGE -
            hprcPayloadBatteryVoltage::MIN_VOLTAGE), 0.0f), 1.0f);
    p->setPen(QPen(m_textBrush, 0));
    p->fillRect(QRect(w->rect().width() / 2.0 - batteryWidth / 2,
        w->rect().height() / 2.0 + batteryHeight / 2 - batteryHeight * batteryCharge, batteryWidth,
        batteryHeight * batteryCharge), batteryBrush);

    p->setPen(QPen(m_textBrush, boundingBox.width() / 100));
    p->drawText(boundingBox, Qt::AlignBottom | Qt::AlignHCenter, valueString + " V");
    p->drawRects(batteryRects);
}