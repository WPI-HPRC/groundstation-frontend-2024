#ifndef HPRCSTYLE_H
#define HPRCSTYLE_H

#include "hprcdisplaywidget.h"
#include <QStyle>
#include <QPainter>

class HPRCStyle
{
public:
    HPRCStyle(const QStyle *style);

    // redraw existing components
    void drawPushButton(QPainter *p, const QStyleOption *o);
    void drawFrame(QPainter *p, const QStyleOption *o);

    // draw custom widgets
    void drawHPRCTimeline(QPainter *p, const hprcDisplayWidget *w);
    void drawHPRCGauge(QPainter *p, const hprcDisplayWidget *w);
    void drawHPRCGraph(QPainter *p, const hprcDisplayWidget *w);

    QBrush m_backgroundBrush;
    QBrush m_panelBrush;
    QBrush m_textBrush;
    QBrush m_highlightBrush;
    QBrush m_buttonBrush;
    QBrush m_transparentBrush;

    QFont m_widgetLarge;
    QFont m_widgetMedium;

    std::map<double, QString> m_stateMap;


private:

};

#endif // HPRCSTYLE_H
