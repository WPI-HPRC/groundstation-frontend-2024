#ifndef HPRCSTYLE_H
#define HPRCSTYLE_H

#include "hprcdisplaywidget.h"
#include <QStyle>
#include <QPainter>
#include "mainwindow.h"

class HPRCStyle
{
public:

    MainWindow::dataPoint* m_latest;

    HPRCStyle(const QStyle *style, MainWindow::dataPoint *d);

    enum HPRCAlarmType
    {
        ALARM_MasterAbort,
        ALARM_Pitch,
        ALARM_MasterWarn,
        ALARM_LowPower,
        ALARM_SignalLoss,
        ALARM_PowerLoss,
        ALARM_MainDeployFail,
        ALARM_DrogueDeployFail,
        ALARM_PayloadDeployFail,
        ALARM_Ballistic,
        ALARM_LowTrack,
        ALARM_HighTrack,
        ALARM_EarlyChute,
        ALARM_Unknown,
    };

    // redraw existing components
    void drawPushButton(QPainter *p, const QStyleOption *o);
    void drawFrame(QPainter *p, const QStyleOption *o);

    // draw custom widgets - backgrounds
    void drawHPRCTimeline(QPainter *p, const hprcDisplayWidget *w);
    void drawHPRCGauge(QPainter *p, const hprcDisplayWidget *w);
    void drawHPRCGraph(QPainter *p, const hprcDisplayWidget *w);
    void drawHPRCAlarmPanel(QPainter *p, const hprcDisplayWidget *w);
    void drawHPRCAlarmFromEnum(QPainter *p, int x, int y, int size, HPRCAlarmType t, bool active, int startX, int startY);
    void drawHPRCClock(QPainter *p, const hprcDisplayWidget *w);

    // draw custom widgets - data sections
    void drawHPRCGaugeData(QPainter *p, const hprcDisplayWidget *w);

    QBrush m_backgroundBrush;
    QBrush m_panelBrush;
    QBrush m_textBrush;
    QBrush m_highlightBrush;
    QBrush m_buttonBrush;
    QBrush m_transparentBrush;

    QFont m_widgetLarge;
    QFont m_widgetMedium;


    // data maps
    std::map<double, QString> m_stateMap;
    std::map<int, int> m_dataMap1;
    std::map<int, int> m_dataMap2;
    std::map<int, int> m_dataMap3;
    std::map<HPRCAlarmType, bool> m_alarmMapW;
    std::map<HPRCAlarmType, bool> m_alarmMapA;




private:

    void drawHPRCSubGraph(QPainter *p, QRectF rect, QColor bg, QList<MainWindow::graphPoint>, double range, double start, const hprcDisplayWidget *w, bool drawTooltip);

};

#endif // HPRCSTYLE_H
