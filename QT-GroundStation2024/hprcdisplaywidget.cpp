#include "hprcdisplaywidget.h"
#include "qapplication.h"
#include "qpainter.h"
#include "qstyle.h"
#include "qstyleoption.h"

hprcDisplayWidget::hprcDisplayWidget(QWidget *parent)
    : QWidget{parent}
{
    m_widgetType = HPRC_Hidden;
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
