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
    //w->widget->drawDetailedView(p, w, &drawResources, m_latest);
    switch (w->widget->getType()) {
    case hprcDisplayWidget::HPRC_Graph:
        drawGraphDetailedView(p, (hprcGraphicsDetailedViewWidget*) w);
        break;
    case hprcDisplayWidget::HPRC_Gauge:
        drawGaugeDetailedView(p, w);
        break;
    case hprcDisplayWidget::HPRC_AIRBRAKES:
        drawAirbrakesDetailedView(p, w);
        break;
    }
}

void HPRCStyle::drawGraphDetailedView(QPainter* p, hprcGraphicsDetailedViewWidget* dw) {
    int width = dw->rect().width();
    int height = dw->rect().height();

    double scaleF = 0.75;
    double paddingRatio = (1 - scaleF) / 2.0;

    int margin = fmin(paddingRatio * width, paddingRatio * height);

    QRectF drawBox = dw->layout()->geometry().adjusted(margin, margin, -margin, -margin);

    dw->graphicsView->setSceneRect(dw->layout()->geometry());

    // label padding = 7.5%
    int lMargin = drawBox.height() * 0.075;
    drawBox.adjust(lMargin, 0, 0, -lMargin);

    QPointF topLeftTop = drawBox.topLeft();
    QPointF bottomRightBottom = drawBox.bottomRight();

    int row2Top = drawBox.y() + drawBox.height() / 3;
    int row3Top = drawBox.y() + 2 * drawBox.height() / 3;

    QPointF bottomRightTop(drawBox.right(), row2Top);
    QPointF topLeftMiddle(drawBox.left(), row2Top);
    QPointF bottomRightMiddle(drawBox.right(), row3Top);
    QPointF topLeftBottom(drawBox.left(), row3Top);

    QRectF top(topLeftTop, bottomRightTop);
    QRectF middle(topLeftMiddle, bottomRightMiddle);
    QRectF bottom(topLeftBottom, bottomRightBottom);

    double start = graphPointCircularBufferGetValueAtIndex(m_latest->altData, 0)->time;
    double range = graphPointCircularBufferGetValueAtIndex(m_latest->altData, m_latest->altData->length - 1)->time - start;


    bool drawT = false;
    if (drawBox.contains(dw->m_mousePos))
    {
        drawT = true;

        dw->m_mousePos.setX(fmin(fmax(dw->m_mousePos.x(), drawBox.x() + TOOLTIP_WIDTH_HALF + 2), drawBox.right() - TOOLTIP_WIDTH_HALF - 2));
    }

    // <---- draw ----> //

    dw->graphicsScene->setBackgroundBrush(m_transparentBrush);

    // Do a little adjusting to help with tooltip rendering
    ((hprcGraph*) dw->widget)->bgRect->setRect(drawBox.adjusted(0, 0, 0, 2));
    ((hprcGraph*)dw->widget)->bgRect->setPen(QPen(m_backgroundBrush, 6));
    ((hprcGraph*)dw->widget)->bgRect->setBrush(m_backgroundBrush);
    ((hprcGraph*)dw->widget)->bgRect->setZValue(-1);

    // Do a little adjusting to help with tooltip rendering
    drawHPRCSubGraph(p, top, m_highlightBrush.color(), m_latest->accData, GRAPH_Acceleration, range, start, ((hprcGraph*) dw->widget), dw->graphicsScene, drawT);
    drawHPRCSubGraph(p, middle.adjusted(0, 1, 0, 1), QColor("#2c4985"), m_latest->velData, GRAPH_Velocity, range, start, ((hprcGraph*)dw->widget), dw->graphicsScene, drawT);
    drawHPRCSubGraph(p, bottom.adjusted(0, 2, 0, 2), QColor("#471d57"), m_latest->altData, GRAPH_Altitude, range, start, ((hprcGraph*)dw->widget), dw->graphicsScene, drawT);

    // Draw an outline to clean up weird border rendering
    ((hprcGraph*)dw->widget)->outlineRect->setRect(drawBox.adjusted(0, 0, 0, 3));
    ((hprcGraph*)dw->widget)->outlineRect->setBrush(m_transparentBrush);
    ((hprcGraph*)dw->widget)->outlineRect->setPen(QPen(m_backgroundBrush, 4));
    ((hprcGraph*)dw->widget)->outlineRect->setZValue(100);

    dw->graphicsView->viewport()->update();
}

void HPRCStyle::drawGaugeDetailedView(QPainter* p, hprcDetailedViewWidget* dw) {
    p->setPen(QPen(QColor(255, 255, 255)));
    p->setBrush(QBrush(QColor(255, 0, 0)));
    p->drawRect(10, 10, 100, 100);
}

void HPRCStyle::drawAirbrakesDetailedView(QPainter* p, hprcDetailedViewWidget* dw) {
    p->setFont(m_widgetMedium);
    p->setPen(QPen(m_textBrush, 3));
    p->drawText(QPoint(100, 100), "Servo");

    //Good idea: put a graph of the airbrakes over time here
}