#include "stylepainters.h"
#include <QApplication>
#include <QStyle>
#include <QPalette>
#include <QStyleOption>
#include <iomanip>
#include <iostream>
#include <QFontDatabase>
#include "Windows/mainwindow.h"
#include <QDateTime>
#include <math.h>
#include <QQuaternion>

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPolygonItem>
#include <QVBoxLayout>

#include "Util/betterqgraphicstextitem.h"
#include "Util/hprcStateMap.h"
#include "Widgets/hprctimeline.h"
#include <Widgets/hprcPayloadBatteryVoltage.h>
#include <Widgets/hprcPayloadGraph.h>
#include <Widgets/hprcViewer.h>
#include <Widgets/hprcPayloadMap.h>

std::map<double, QString> hprcStateMaps::stateMap;
std::map<HPRCStyle::HPRCAlarmType, bool> hprcStateMaps::alarmMapW;
std::map<HPRCStyle::HPRCAlarmType, bool> hprcStateMaps::alarmMapA;


HPRCStyle::HPRCStyle(const QStyle *style, MainWindow::dataPoint *d) {
    QPalette widgetPalette = style->standardPalette();
    m_backgroundBrush = widgetPalette.brush(QPalette::Window);
    m_panelBrush = widgetPalette.brush(QPalette::Base);
    m_textBrush = widgetPalette.brush(QPalette::Text);
    m_highlightBrush = widgetPalette.brush(QPalette::Highlight);
    m_buttonBrush = widgetPalette.brush(QPalette::Button);
    m_transparentBrush = *new QBrush(QColor(255, 255, 255, 0));

    drawResources.m_backgroundBrush = widgetPalette.brush(QPalette::Window);
    drawResources.m_panelBrush = widgetPalette.brush(QPalette::Base);
    drawResources.m_textBrush = widgetPalette.brush(QPalette::Text);
    drawResources.m_highlightBrush = widgetPalette.brush(QPalette::Highlight);
    drawResources.m_buttonBrush = widgetPalette.brush(QPalette::Button);
    drawResources.m_transparentBrush = *new QBrush(QColor(255, 255, 255, 0));

    int id = QFontDatabase::addApplicationFont(":/Fonts/OverpassMono-VariableFont_wght.ttf");
    QString overpassMono = QFontDatabase::applicationFontFamilies(id).at(0);

    id = QFontDatabase::addApplicationFont(":/Fonts/JockeyOne-Regular.ttf");
    QStringList l = QFontDatabase::applicationFontFamilies(id);
    QString jockeyOne = QFontDatabase::applicationFontFamilies(id).at(0);
    m_widgetFancy = QFont(jockeyOne, 30, 5, false);
    m_widgetLarge = QFont(overpassMono, 20, 5, false);
    m_widgetMedium = QFont(overpassMono, 15, 5, false);
    m_widgetSmall = QFont(overpassMono, 10, 5, false);

    drawResources.m_widgetFancy = QFont(jockeyOne, 30, 5, false);
    drawResources.m_widgetLarge = QFont(overpassMono, 20, 5, false);
    drawResources.m_widgetMedium = QFont(overpassMono, 15, 5, false);
    drawResources.m_widgetSmall = QFont(overpassMono, 10, 5, false);



    // get a pointer to the current data's location

    m_latest = d;
}


void HPRCStyle::drawPushButton(QPainter *p, const QStyleOption *o) {

    double borderWidth = 4;

    QPen borderPen(m_highlightBrush, borderWidth);
    p->setRenderHint(QPainter::Antialiasing);
    p->setPen(borderPen);
    p->setBrush(m_buttonBrush);
    p->drawRoundedRect(
            QRectF(o->rect).adjusted(borderWidth - 0.5, borderWidth - 0.5, 0.5 - borderWidth, 0.5 - borderWidth),
            o->rect.height() / 2, o->rect.height() / 2);
}

void HPRCStyle::drawFrame(QPainter *p, const QStyleOption *o) {
    p->setRenderHint(QPainter::Antialiasing);
    p->setBrush(m_panelBrush);
    p->setPen(QPen(m_transparentBrush, 1));
    p->drawRoundedRect(QRectF(o->rect).adjusted(0.5, 0.5, -0.5, -0.5), 10, 10);

}

//Does not build if this function goes inside the detailed view file for some reason
void HPRCStyle::drawDetailedView(QPainter* p, hprcDetailedViewWidget* w) {
    w->widget->drawDetailedView(p, &drawResources, m_latest);
}