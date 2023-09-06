#include "stylepainters.h"
#include <QApplication>
#include <QStyle>
#include <QPalette>
#include <QStyleOption>
#include <iostream>
#include <QFontDatabase>

HPRCStyle::HPRCStyle(const QStyle *style)
{
    QPalette widgetPalette = style->standardPalette();
    m_backgroundBrush = widgetPalette.brush(QPalette::Window);
    m_panelBrush = widgetPalette.brush(QPalette::Base);
    m_textBrush = widgetPalette.brush(QPalette::Text);
    m_highlightBrush = widgetPalette.brush(QPalette::Highlight);
    m_buttonBrush = widgetPalette.brush(QPalette::Button);
    m_transparentBrush = *new QBrush(QColor(255, 255, 255, 0));

    int id = QFontDatabase::addApplicationFont(":/fonts/fonts/OverpassMono-VariableFont_wght.ttf");
    QString overpassMono = QFontDatabase::applicationFontFamilies(id).at(0);
    m_widgetLarge = QFont(overpassMono, 20, 5, false);
    m_widgetMedium = QFont(overpassMono, 15, 5, false);

    double ticks[5] = {1, 0.85, 0.5, 0.25, 0}; // these need to be ordered bottom to top.  the system draws from top-left so "1" is the bottom (top - 1 * height)
    QString states[4] = {"BOOST", "CRUISE", "DROGUE", "MAIN"}; // will be associated with the space between [n] and [n+1] eg BOOST is between 1 and 0.85
    for(int index = 0; index < (sizeof(ticks)/sizeof(ticks[0])) - 1; index++)
    {
        double start = ticks[index];
        double end = ticks[index + 1];
        double middle = (start - end) / 2.0 + end;
        m_stateMap.insert(std::make_pair(start, QString("-")));
        m_stateMap.insert(std::make_pair(middle, states[index]));

    }
    m_stateMap.insert(std::make_pair(ticks[sizeof(ticks)/sizeof(ticks[0]) - 1], QString("-")));
}

void HPRCStyle::drawPushButton(QPainter *p, const QStyleOption *o)
{

    double borderWidth = 4;

    QPen borderPen(m_highlightBrush, borderWidth);
    p->setRenderHint(QPainter::Antialiasing);
    p->setPen(borderPen);
    p->setBrush(m_buttonBrush);
    p->drawRoundedRect(QRectF(o->rect).adjusted(borderWidth - 0.5, borderWidth - 0.5, 0.5 - borderWidth, 0.5 - borderWidth), o->rect.height()/2, o->rect.height()/2);
}

void HPRCStyle::drawFrame(QPainter *p, const QStyleOption *o)
{
    p->setRenderHint(QPainter::Antialiasing);
    p->setBrush(m_panelBrush);
    p->setPen(QPen(m_transparentBrush, 0));
    p->drawRoundedRect(QRectF(o->rect).adjusted(0.5, 0.5, -0.5, -0.5), 10, 10);

}

void HPRCStyle::drawHPRCTimeline(QPainter *p, const hprcDisplayWidget *w)
{

    // <------------------- math --------------------->//



    p->setRenderHint(QPainter::Antialiasing);

    p->setFont(m_widgetLarge);
    p->setBrush(m_transparentBrush);
    QPen bgPen(m_backgroundBrush, 15);
    bgPen.setCapStyle(Qt::RoundCap);

    QPen fgPen(m_highlightBrush, 16);
    fgPen.setCapStyle(Qt::RoundCap);

    QPen tickPen(m_textBrush, 20);
    tickPen.setCapStyle(Qt::FlatCap);

    double aspect = 4; // height / width

    QRect widgetBox = w->rect();

    double drawWidth = widgetBox.width();
    double drawHeight = widgetBox.height();

    // wanted aspect ratio is 1:3
    if(drawHeight / drawWidth < aspect)
    {
        drawWidth = drawHeight/aspect;
    } else {
        drawHeight = drawWidth*aspect;
    }

    drawHeight *= 0.85;
    drawWidth  *= 0.85;

    m_widgetLarge.setPointSize(drawHeight/30);
    m_widgetMedium.setPointSize(drawHeight/30 * 0.75);

    fgPen.setWidth(drawHeight/40 + 1);
    bgPen.setWidth(drawHeight/40);

    double drawX = (widgetBox.width() - drawWidth) / 2.0;
    double drawY = (widgetBox.height() - drawHeight) / 2.0; // I dunno but it works

    double scaleF = 0.03;

    QRect drawBox(drawX, drawY, drawWidth, drawHeight);
    QPoint bottomRight(drawX + (0.9 - scaleF) * drawWidth, drawBox.bottom());
    QPoint topLeftSlot(drawX + (0.9 - scaleF) * drawWidth, drawY);
    QPoint topLeftFill(drawX + (0.9 - scaleF) * drawWidth, drawY + drawHeight - (drawHeight * (w->m_filledPercent / 100.0)));

    QRect timelineSlot(topLeftSlot, bottomRight);
    QRect timelineFill(topLeftFill, bottomRight);

    // <------------------- draw --------------------->//

    p->setPen(bgPen);
    p->drawRoundedRect(timelineSlot, (scaleF*drawWidth/2), (scaleF*drawWidth/2));
    p->setPen(fgPen);
    if(w->m_filledPercent > 1)
        p->drawRoundedRect(timelineFill, (scaleF*drawWidth/2), (scaleF*drawWidth/2));
    p->setPen(tickPen);

    for(const auto& [position, label] : m_stateMap)
    {
        QTransform rPt(1, 0, 0, 1, topLeftSlot.x() - (p->font().pointSize()) * label.length(), position * drawHeight + drawY + p->font().pointSize() * 0.3);
        p->setTransform(rPt);
        p->drawText(0, 0, label);

        if(label.startsWith("-"))
        {
            p->setPen(bgPen);
            p->setFont(m_widgetMedium);
            p->drawText(p->font().pointSize() * -8 + 20, 0, QString("00:00:00"));
            p->setFont(m_widgetLarge);
            p->setPen(tickPen);
        }
    }


}

void HPRCStyle::drawHPRCGauge(QPainter *p, const hprcDisplayWidget *w)
{
    p->setRenderHint(QPainter::Antialiasing);
    p->setFont(m_widgetLarge);
    p->setBrush(m_backgroundBrush);
    QPen bgPen(m_backgroundBrush, 15);
    bgPen.setCapStyle(Qt::RoundCap);
    p->setPen(bgPen);

    QRectF boundingBox(w->rect().adjusted(15, 15, -15, -15));

    double scaleF = 0.85;

    int oWidth = boundingBox.width();

    int sizeMin = fmin(boundingBox.height(), boundingBox.width() * scaleF);

    boundingBox.adjust(oWidth/2 - sizeMin/2, 0, oWidth/2 - sizeMin/2, 0);

    boundingBox.setHeight(sizeMin);
    boundingBox.setWidth(sizeMin);


    int extraArc = 35;

    p->drawArc(boundingBox, -extraArc*16, (180 + 2 * extraArc)*16);



//    p->drawRect(drawRect);


}


