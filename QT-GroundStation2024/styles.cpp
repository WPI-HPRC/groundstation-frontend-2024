#include "styles.h"
#include "hprcdisplaywidget.h"
#include "stylepainters.h"
#include <iostream>

//<--------------------Style Color Definitions--------------------->//

QColor darkBackground("#141414");
QColor darkPanel("#212121");
QColor darkText("#ffffff");
QColor darkHighlight("#AF283A");
QColor darkButton("#212121");

QColor lightBackground("#607d8b");
QColor lightPanel("#E0E0E0");
QColor lightText("#000000");
QColor lightHighlight("#AF283A");
QColor lightButton("#BDBDBD");

Style::Style(QColor *background, QColor *panel, QColor *text, QColor *highlight, QColor *button)
{
    m_background = *background;
    m_panel = *panel;
    m_text = *text;
    m_highlight = *highlight;
    m_button = *button;
}

void Style::drawControl(QStyle::ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    HPRCStyle drawer;
    switch(element)
    {
    case QStyle::CE_PushButtonBevel:
        drawer.drawPushButton(painter, option);
        return;
    default:
        QProxyStyle::drawControl(element, option, painter, widget);
        return;
    }
}

void Style::drawPrimitive(QStyle::PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    HPRCStyle drawer;
    switch(element)
    {
    case QStyle::PE_CustomBase:
        std::cerr << "found a widget \n";
        if(const hprcDisplayWidget* w = dynamic_cast<const hprcDisplayWidget*>(widget))
        {
            std::cerr << "found a cool widget \n";
            std::cerr << w->getType();
        }
        return;
    case QStyle::PE_Frame:
        drawer.drawFrame(painter, option);
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

HPRCLightStyle::HPRCLightStyle() : Style(&lightBackground, &lightPanel, &lightText, &lightHighlight, &lightButton) {}

HPRCDarkStyle::HPRCDarkStyle() : Style(&darkBackground, &darkPanel, &darkText, &darkHighlight, &darkButton) {}
