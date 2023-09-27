#include "hprcdisplaywidget.h"
#include "mainwindow.h"
#include "qapplication.h"
#include "qpainter.h"
#include "qstyle.h"
#include "qstyleoption.h"
#include <iostream>
#include "QPropertyAnimation"
#include <QMouseEvent>
#include <QHBoxLayout>

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

hprcGaugeData::hprcGaugeData(QWidget *parent)
    : hprcDisplayWidget{parent}
{
    m_widgetType = HPRC_GaugeData;
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
    QHBoxLayout* hLayout = new QHBoxLayout;
    this->setLayout(hLayout);
    hLayout->setContentsMargins(0, 0, 0, 0);
    this->setContentsMargins(0, 0, 0, 0);
    hprcAltitudeGaugeData* dataWidget = new hprcAltitudeGaugeData(this);
    hLayout->addWidget(dataWidget);

    m_label = "ALTITUDE";
}

hprcAltitudeGaugeData::hprcAltitudeGaugeData(QWidget *parent) :
    hprcGaugeData{parent}
{
    m_max = 12000.0;
    foreach (QWidget *w, qApp->topLevelWidgets())
        if (MainWindow* mainWin = qobject_cast<MainWindow*>(w))
        {
            connect(mainWin, SIGNAL(altUpdated(int)), this, SLOT(updateFilled(int)));
        }
}

hprcVelocityGauge::hprcVelocityGauge(QWidget *parent) :
    hprcGauge{parent}
{
    QHBoxLayout* hLayout = new QHBoxLayout;
    this->setLayout(hLayout);
    hLayout->setContentsMargins(0, 0, 0, 0);
    this->setContentsMargins(0, 0, 0, 0);
    hprcVelocityGaugeData* dataWidget = new hprcVelocityGaugeData(this);
    hLayout->addWidget(dataWidget);

    m_label = "VELOCITY";

}

hprcVelocityGaugeData::hprcVelocityGaugeData(QWidget *parent) :
    hprcGaugeData{parent}
{
    m_max = 1000.0;
    foreach (QWidget *w, qApp->topLevelWidgets())
        if (MainWindow* mainWin = qobject_cast<MainWindow*>(w))
        {
            connect(mainWin, SIGNAL(velUpdated(int)), this, SLOT(updateFilled(int)));
        }
}

hprcAccelerationGauge::hprcAccelerationGauge(QWidget *parent) :
    hprcGauge{parent}
{
    QHBoxLayout* hLayout = new QHBoxLayout;
    this->setLayout(hLayout);
    hLayout->setContentsMargins(0, 0, 0, 0);
    this->setContentsMargins(0, 0, 0, 0);
    hprcAccelerationGaugeData* dataWidget = new hprcAccelerationGaugeData(this);
    hLayout->addWidget(dataWidget);


    m_label = "ACCELERATION";

}

hprcAccelerationGaugeData::hprcAccelerationGaugeData(QWidget *parent) :
    hprcGaugeData{parent}
{
    m_max = 400;
    foreach (QWidget *w, qApp->topLevelWidgets())
        if (MainWindow* mainWin = qobject_cast<MainWindow*>(w))
        {
            connect(mainWin, SIGNAL(accUpdated(int)), this, SLOT(updateFilled(int)));
        }
}

hprcGraph::hprcGraph(QWidget *parent) :
    hprcDisplayWidget(parent)
{
    setMouseTracking(true);
    m_widgetType = HPRC_Graph;
    foreach (QWidget *w, qApp->topLevelWidgets())
        if (MainWindow* mainWin = qobject_cast<MainWindow*>(w))
        {
            connect(mainWin, SIGNAL(tick()), this, SLOT(repaint()));
        }
}

void hprcGraph::mouseMoveEvent(QMouseEvent *e)
{
    m_mousePos = e->pos();

}

hprcAlarmPanel::hprcAlarmPanel(QWidget *parent) :
    hprcDisplayWidget(parent)
{
    m_widgetType = HPRC_Alarm;
}

hprcClock::hprcClock(QWidget *parent) :
    hprcDisplayWidget(parent)
{
    // subscribe to any data that needs to cause an update
    foreach (QWidget *w, qApp->topLevelWidgets())
        if (MainWindow* mainWin = qobject_cast<MainWindow*>(w))
        {
            connect(mainWin, SIGNAL(tick()), this, SLOT(repaint()));
        }


    m_widgetType = HPRC_Clock;
}
