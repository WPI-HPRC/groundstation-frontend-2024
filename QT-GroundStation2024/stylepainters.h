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

    enum rocketLabel
    {
        LABEL_Motor,
        LABEL_Airbrakes,
        LABEL_PayloadBay,
        LABEL_AvionicsBay,
        LABEL_RecoveryBay,
        LABEL_Other,
    };

    // redraw existing components
    void drawPushButton(QPainter *p, const QStyleOption *o);
    void drawFrame(QPainter *p, const QStyleOption *o);

    // draw custom widgets
    void drawHPRCTimeline(QPainter *p, const hprcDisplayWidget *w);
    void drawHPRCGauge(QPainter *p, const hprcDisplayWidget *w);
    void drawHPRCAttitudeWidget(QPainter* p, const hprcDisplayWidget *w);
    void drawHPRCGraph(QPainter *p, const hprcDisplayWidget *w);
    void drawHPRCPayloadGraph(QPainter *p, const hprcDisplayWidget *w);
    void drawHPRCAlarmPanel(QPainter *p, const hprcDisplayWidget *w);
    void drawHPRCAlarmFromEnum(QPainter *p, int x, int y, int size, HPRCAlarmType t, bool active, int startX, int startY);
    void drawHPRCClock(QPainter *p, const hprcDisplayWidget *w);
    void drawHPRCRocketVis(QPainter *p, const hprcDisplayWidget *w);
    void drawHPRCAirbrakes(QPainter* p, const hprcDisplayWidget* w);
    void drawHPRCViewer(QPainter *p, const hprcDisplayWidget *w);
    void drawServoStatusServo(QPainter* p, const hprcDisplayWidget* w, QString title, float encoderPosition, float x, float y, float width);
    void drawHprcServoStatus(QPainter *p, const hprcDisplayWidget *w);


    QBrush m_backgroundBrush;
    QBrush m_panelBrush;
    QBrush m_textBrush;
    QBrush m_highlightBrush;
    QBrush m_buttonBrush;
    QBrush m_transparentBrush;

    QFont m_widgetLarge;
    QFont m_widgetMedium;
    QFont m_widgetSmall;


    // data maps
    std::map<double, QString> m_stateMap;
    std::map<int, int> m_dataMap1;
    std::map<int, int> m_dataMap2;
    std::map<int, int> m_dataMap3;
    std::map<HPRCAlarmType, bool> m_alarmMapW;
    std::map<HPRCAlarmType, bool> m_alarmMapA;


    float m_AttitudeMaxDegreeRange = 15;
    float m_AttitudeDegreeOffsetYaw = 0;
    float m_AttitudeDegreeOffsetPitch = 0;

    std::vector<std::vector<float>> m_AttitudeCircleLocationsDegrees {
        std::vector<float> {
            3.0, 7.0, 12.0, 15.0
        }
    };


private:

private:
    struct Range {
        double min;
        double max;
    };

    static constexpr float AIRBRAKE_DESIRED_THRESHOLD = 0.075f; //The minimum amount of difference between the desired and current position required to display the desired airbrakes

    void drawHPRCSubGraph(QPainter *p, QRectF rect, QColor bg, QList<MainWindow::graphPoint>*, double range, double start, const hprcDisplayWidget *w, bool drawTooltip);
    Range drawHPRCSubGraph(QPainter *p, QRectF rect, QColor bg, QList<MainWindow::graphPoint>*, double range, double start, const hprcDisplayWidget *w, bool drawTooltip, double lowerBound, double upperBound, bool enableEndZeroPoints, bool enablePolygonTransformationRendering, int* startIndex, QPolygonF* polygon); //startIndex pointer means that you give a pointer to the start index variable for this piece of data
    void drawHPRCRocketLabel(QPainter *p, rocketLabel l, QPointF target, QPointF label);
    Range getDataYRange(QList<MainWindow::graphPoint>* data);
};

#endif // HPRCSTYLE_H
