#include "hprcdisplaywidget.h"
#include "qapplication.h"
#include "qpainter.h"
#include "qstyle.h"
#include "qstyleoption.h"
#include <iostream>
#include "QPropertyAnimation"

hprcDisplayWidget::hprcDisplayWidget(QWidget *parent)
    : QWidget{parent}
{
    m_widgetType = HPRC_Hidden;
    m_filledPercent = 0;
    m_label = QString("TESTING");
}

void hprcDisplayWidget::paintEvent(QPaintEvent *e)
{
    QPainter p(this);
    const QStyleOption opt(QStyleOption::Version, QStyleOption::SO_Default);
    QApplication::style()->drawPrimitive(QStyle::PE_CustomBase, &opt, &p, this);
}

hprcTimeline::hprcTimeline(QWidget *parent)
    : hprcDisplayWidget{parent}
{
    m_widgetType = HPRC_Timeline;
}

hprcGauge::hprcGauge(QWidget *parent)
    : hprcDisplayWidget{parent}
{
    m_widgetType = HPRC_Gauge;
}

void hprcDisplayWidget::updateFilled(int input)
{

    QPropertyAnimation *animation = new QPropertyAnimation(this, "filledPercent");
    animation->setDuration(400);
    animation->setEndValue(input);
    animation->start();
}

int hprcDisplayWidget::getFilled()
{

    return m_filledPercent;
}
void hprcDisplayWidget::setFilled(int input)
{
    m_filledPercent = input;
    update();
}
int hprcDisplayWidget::fillChanged()
{

    return m_filledPercent;
}

hprcAltitudeGauge::hprcAltitudeGauge(QWidget *parent) :
    hprcGauge{parent}
{
    m_label = "ALTITUDE";
}

hprcVelocityGauge::hprcVelocityGauge(QWidget *parent) :
    hprcGauge{parent}
{
    m_label = "VELOCITY";
}

hprcAccelerationGauge::hprcAccelerationGauge(QWidget *parent) :
    hprcGauge{parent}
{
    m_label = "ACCELERATION";
}

hprcGraph::hprcGraph(QWidget *parent) :
    hprcDisplayWidget(parent)
{
    m_widgetType = HPRC_Graph;
}

hprcAlarmPanel::hprcAlarmPanel(QWidget *parent) :
    hprcDisplayWidget(parent)
{
    m_widgetType = HPRC_Alarm;
}
