#include "styles.h"
#include "hprcdisplaywidget.h"
#include "stylepainters.h"
#include <iostream>

//<--------------------Style Color Definitions--------------------->//

QColor darkBackground(20, 20, 20); //#141414
QColor darkPanel(33, 33, 33); // #212121
QColor darkText(255, 255, 255); // #FFFFFF
QColor darkHighlight(175, 40, 58); // #AF283A
QColor darkButton(33, 33, 33); // #212121

QColor lightBackground(96, 125, 139); //607D8B
QColor lightPanel(224, 224, 244); // #E0E0E0
QColor lightText(0, 0, 0); // #000000
QColor lightHighlight(175, 40, 58); // #AF283A
QColor lightButton(189, 189, 189); // #BDBDBD

Style::Style(QColor *background, QColor *panel, QColor *text, QColor *highlight, QColor *button, MainWindow::dataPoint *d)
{
    m_background = *background;
    m_panel = *panel;
    m_text = *text;
    m_highlight = *highlight;
    m_button = *button;
    m_drawer = new HPRCStyle(this, d);
}

void Style::drawControl(QStyle::ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    switch(element)
    {
    case QStyle::CE_PushButtonBevel:
        m_drawer->drawPushButton(painter, option);
        return;
    default:
        QProxyStyle::drawControl(element, option, painter, widget);
        return;
    }
}

void Style::drawPrimitive(QStyle::PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    switch(element)
    {
    case QStyle::PE_CustomBase:
        if(const hprcDisplayWidget* w = dynamic_cast<const hprcDisplayWidget*>(widget))
        {
            switch(w->getType())
            {
            case hprcDisplayWidget::HPRC_Timeline:
                m_drawer->drawHPRCTimeline(painter, w);
                return;
            case hprcDisplayWidget::HPRC_Gauge:
                m_drawer->drawHPRCGauge(painter, w);
                return;
            case hprcDisplayWidget::HPRC_GaugeData:
                m_drawer->drawHPRCGaugeData(painter, w);
                return;
            case hprcDisplayWidget::HPRC_Graph:
                m_drawer->drawHPRCGraph(painter, w);
                return;
            case hprcDisplayWidget::HPRC_Alarm:
                m_drawer->drawHPRCAlarmPanel(painter, w);
                return;
            case hprcDisplayWidget::HPRC_Clock:
                m_drawer->drawHPRCClock(painter, w);
                return;
            case hprcDisplayWidget::HPRC_RocketVisual:
                m_drawer->drawHPRCRocketVis(painter, w);
            default:
                return;
            }
        }
        return;
    case QStyle::PE_Frame:
        m_drawer->drawFrame(painter, option);
        return;
    default:
        QProxyStyle::drawPrimitive(element, option, painter, widget);
        return;
    }
}

QPalette Style::standardPalette() const
{
    QPalette returnPal;

    returnPal.setBrush(QPalette::Window, *new QBrush(m_background));
    returnPal.setBrush(QPalette::Base, *new QBrush(m_panel));
    returnPal.setBrush(QPalette::Text, *new QBrush(m_text));
    returnPal.setBrush(QPalette::Highlight, *new QBrush(m_highlight));
    returnPal.setBrush(QPalette::Button, *new QBrush(m_button));

    return returnPal;
}

HPRCLightStyle::HPRCLightStyle(MainWindow::dataPoint *d) : Style(&lightBackground, &lightPanel, &lightText, &lightHighlight, &lightButton, d) {}

HPRCDarkStyle::HPRCDarkStyle(MainWindow::dataPoint *d) : Style(&darkBackground, &darkPanel, &darkText, &darkHighlight, &darkButton, d) {}
