#ifndef HPRCSTYLE_H
#define HPRCSTYLE_H

#include <QStyle>
#include <QPainter>

class HPRCStyle
{
public:
    HPRCStyle();

    void drawPushButton(QPainter *p, const QStyleOption *o);
    void drawWindowBackground(QPainter *p, const QStyleOption *o);
    void drawFrame(QPainter *p, const QStyleOption *o);

    QBrush m_backgroundBrush;
    QBrush m_panelBrush;
    QBrush m_textBrush;
    QBrush m_highlightBrush;
    QBrush m_buttonBrush;
    QBrush m_transparentBrush;
};

#endif // HPRCSTYLE_H
