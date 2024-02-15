#ifndef HPRCSTYLE_H
#define HPRCSTYLE_H

#include "Widgets/hprcdisplaywidget.h"
#include "Widgets/hprcgraph.h"
#include <QStyle>
#include <QPainter>
#include "Windows/mainwindow.h"
#include "Widgets/hprctimeline.h"

#define TOOLTIP_WIDTH 50
#define TOOLTIP_WIDTH_HALF 25

#define NUM_NAVBALL_CIRCLES 7

#define MAX_GRAPH_SCALE 0.85
#define GRAPH_TICK_DISTANCE 50
#define MAX_DYNAMIC_GRAPH_SCALE 600

#define MAX_GRAPH_SCALE 0.85
#define GRAPH_TICK_DISTANCE 50
#define MAX_DYNAMIC_GRAPH_SCALE 600

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

    enum rocketLabel
    {
        LABEL_Motor,
        LABEL_Airbrakes,
        LABEL_PayloadBay,
        LABEL_AvionicsBay,
        LABEL_RecoveryBay,
        LABEL_Other,
    };

    enum GraphType
    {
        GRAPH_Altitude,
        GRAPH_Velocity,
        GRAPH_Acceleration
    };

    // redraw existing components
    void drawPushButton(QPainter *p, const QStyleOption *o);
    void drawFrame(QPainter *p, const QStyleOption *o);

    // draw custom widgets
    void drawHPRCTimeline(QPainter *p, const hprcTimeline *w);
    void drawHPRCGauge(QPainter *p, const hprcDisplayWidget *w);
    void drawHPRCAttitudeWidget(QPainter* p, const hprcDisplayWidget *w);
    void drawHPRCGraph(QPainter *p, hprcGraph *w);
    void drawHPRCAlarmPanel(QPainter *p, const hprcDisplayWidget *w);
    void drawHPRCAlarmFromEnum(QPainter *p, int x, int y, int size, HPRCAlarmType t, bool active, int startX, int startY);
    void drawHPRCClock(QPainter *p, const hprcDisplayWidget *w);
    void drawHPRCRocketVis(QPainter *p, const hprcDisplayWidget *w);
    void drawHPRCAirbrakes(QPainter* p, const hprcDisplayWidget* w);
    void drawHPRCViewer(QPainter *p, const hprcDisplayWidget *w);


    QBrush m_backgroundBrush;
    QBrush m_panelBrush;
    QBrush m_textBrush;
    QBrush m_highlightBrush;
    QBrush m_buttonBrush;
    QBrush m_transparentBrush;

    QFont m_widgetLarge;
    QFont m_widgetMedium;
    QFont m_widgetSmall;
    QFont m_widgetFancy;

    float m_AttitudeMaxDegreeRange = 15;
    float m_AttitudeDegreeOffsetYaw = 0;
    float m_AttitudeDegreeOffsetPitch = 0;

    std::vector<std::vector<float>> m_AttitudeCircleLocationsDegrees {

        std::vector<float> {
            3.0, 7.0, 12.0, 15.0
        },
        std::vector<float> {
            3.0, 7.0, 12.0, 15.0
        },
        std::vector<float> {
            3.0, 7.0, 12.0, 15.0
        },
        std::vector<float> {
            3.0, 7.0, 12.0, 15.0
        },
        std::vector<float> {
            3.0, 7.0, 12.0, 15.0
        },
        std::vector<float> {
            3.0, 7.0, 12.0, 15.0
        },
        std::vector<float> {
            3.0, 7.0, 12.0, 15.0
        },
        std::vector<float> {
            3.0, 7.0, 12.0, 15.0
        }
    };


private:

private:
    static constexpr float AIRBRAKE_DESIRED_THRESHOLD = 0.075f; //The minimum amount of difference between the desired and current position required to display the desired airbrakes

    void drawHPRCSubGraph(QPainter *p, QRectF rect, QColor bg, GraphPointCircularBuffer *data, GraphType graphType,  double range, double start, hprcGraph *w, QGraphicsScene* scene, bool drawTooltip);
    void drawHPRCRocketLabel(QPainter *p, rocketLabel l, QPointF target, QPointF label);
};

#endif // HPRCSTYLE_H
