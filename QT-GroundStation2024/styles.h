#pragma once

#include "stylepainters.h"
#include <QObject>
#include <QProxyStyle>

class Style : public QProxyStyle
{
    Q_OBJECT
public:

    QPalette standardPalette() const override;
    void drawControl(QStyle::ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget = nullptr) const override;
    void drawPrimitive(QStyle::PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget = nullptr) const override;
    Style(QColor *background, QColor *panel, QColor *text, QColor *highlight, QColor *button, MainWindow::dataPoint *d);


private:

    QColor m_background;
    QColor m_panel;
    QColor m_text;
    QColor m_highlight;
    QColor m_button;

    HPRCStyle *m_drawer;
};

class HPRCLightStyle : public Style
{
    Q_OBJECT
public:
    HPRCLightStyle(MainWindow::dataPoint *d);
};
class HPRCDarkStyle : public Style
{
    Q_OBJECT
public:
    HPRCDarkStyle(MainWindow::dataPoint *d);
};