#include "hprcGauge.h"
#include "Windows/mainwindow.h"

#include "stylepainters.h"

#include <QApplication>
#include <QPen>
#include <QPainter>

hprcGauge::hprcGauge(QWidget* parent)
    : hprcDisplayWidget{ parent }
{
    m_widgetType = HPRC_Gauge;
    detailedViewEnabled = true;
}

hprcAltitudeGauge::hprcAltitudeGauge(QWidget* parent) :
    hprcGauge{ parent }
{
    m_max = 12000.0;
    m_label = "ALTITUDE";
    foreach(QWidget * w, qApp->topLevelWidgets())
        if (MainWindow* mainWin = qobject_cast<MainWindow*>(w))
        {
            connect(mainWin, SIGNAL(altUpdated(float, float)), this, SLOT(updateFilled(float)));
        }
}

hprcVelocityGauge::hprcVelocityGauge(QWidget* parent) :
    hprcGauge{ parent }
{
    m_max = 1000.0;
    m_label = "VELOCITY";
    foreach(QWidget * w, qApp->topLevelWidgets())
        if (MainWindow* mainWin = qobject_cast<MainWindow*>(w))
        {
            connect(mainWin, SIGNAL(velUpdated(int)), this, SLOT(updateFilled(int)));
        }
}

hprcAccelerationGauge::hprcAccelerationGauge(QWidget* parent) :
    hprcGauge{ parent }
{
    m_label = "ACCELERATION";
    m_max = 400;
    foreach(QWidget * w, qApp->topLevelWidgets())
        if (MainWindow* mainWin = qobject_cast<MainWindow*>(w))
        {
            connect(mainWin, SIGNAL(accUpdated(int)), this, SLOT(updateFilled(int)));
        }
}

hprcRollGauge::hprcRollGauge(QWidget* parent) :
    hprcGauge{ parent }
{
    m_label = "ROLL";
    m_max = 100;
    foreach(QWidget * w, qApp->topLevelWidgets())
        if (MainWindow* mainWin = qobject_cast<MainWindow*>(w))
        {
            connect(mainWin, SIGNAL(rollUpdated(int)), this, SLOT(updateFilled(int)));
        }
}

hprcPitchGauge::hprcPitchGauge(QWidget* parent) :
    hprcGauge{ parent }
{
    m_label = "PITCH";
    m_max = 100;
    foreach(QWidget * w, qApp->topLevelWidgets())
        if (MainWindow* mainWin = qobject_cast<MainWindow*>(w))
        {
            connect(mainWin, SIGNAL(pitchUpdated(int)), this, SLOT(updateFilled(int)));
        }
}

void HPRCStyle::drawHPRCGauge(QPainter* p, const hprcDisplayWidget* w) {
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

    boundingBox.adjust(oWidth / 2 - sizeMin / 2, 0, oWidth / 2 - sizeMin / 2, 0);

    boundingBox.setHeight(sizeMin);
    boundingBox.setWidth(sizeMin);

    int extraArc = 35;


    QConicalGradient progressGradient(boundingBox.center(),
        (180 + extraArc - 5) - (w->m_filledPercent / w->m_max) * (180 + 2 * extraArc));
    progressGradient.setColorAt(1, m_panelBrush.color());
    progressGradient.setColorAt(0, m_highlightBrush.color());

    QPen fgPen(m_highlightBrush, 5);

    fgPen.setCapStyle(Qt::RoundCap);

    bgPen.setWidth(sizeMin / 10);
    fgPen.setWidth(sizeMin / 10 - 5);

    QString dataString = QString::number(w->m_filledPercent);
    bool negative = false;
    if (dataString.contains("-")) {
        dataString.remove("-");
        negative = true;
    }
    while (dataString.size() < 4) {
        dataString.prepend("0");
    }
    if (negative)
        dataString.prepend("-");

    m_widgetLarge.setPointSize(sizeMin / 13);
    p->setFont(m_widgetLarge);

    // <---- draw ----> //

    p->setPen(bgPen);
    p->drawArc(boundingBox, -extraArc * 16, (180 + 2 * extraArc) * 16);
    p->setPen(fgPen);
    p->drawArc(boundingBox, (180 * 16 + extraArc * 16),
        (180 + 2 * extraArc) * -16 * (fmax(0, fmin(w->m_filledPercent / w->m_max, 1))));
    p->setPen(textPen);
    p->drawText(boundingBox.adjusted(0, 30, 0, 30), Qt::AlignCenter, w->m_label);

    m_widgetLarge.setPointSize(sizeMin / 7);
    p->setFont(m_widgetLarge);

    p->drawText(boundingBox.adjusted(0, -30, 0, -30), Qt::AlignCenter, dataString);
}