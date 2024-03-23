#include "hprcGauge.h"
#include "Windows/mainwindow.h"

#include <QApplication>
#include <QPen>
#include <QPainter>

hprcGauge::hprcGauge(QWidget* parent)
    : hprcDisplayWidget{ parent }
{
    m_widgetType = HPRC_Gauge;
    detailedViewEnabled = true;
}

void hprcGauge::drawDetailedView(QPainter* p) {
    p->setPen(QPen(QColor(255, 255, 255)));
    p->setBrush(QBrush(QColor(255, 0, 0)));
    p->drawRect(10, 10, 100, 100);
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