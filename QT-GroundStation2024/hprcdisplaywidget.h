#ifndef HPRCDISPLAYWIDGET_H
#define HPRCDISPLAYWIDGET_H

#include "mainwindow.h"
#include <QObject>
#include <QWidget>

#include <Qt3DCore/QTransform>
#include <Qt3DExtras/Qt3DWindow>
#include <Qt3DExtras/QDiffuseSpecularMaterial>
#include <QQuaternion>
#include <QColor>

class hprcDisplayWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int filledPercent READ getFilled WRITE setFilled NOTIFY fillChanged MEMBER m_filledPercent)

public:
    explicit hprcDisplayWidget(QWidget *parent = nullptr);

    enum hprcWidgetType
    {
        HPRC_Timeline,
        HPRC_Gauge,
        HPRC_Attitude,
        HPRC_Graph,
        HPRC_PAYLOAD_GRAPH,
        HPRC_Alarm,
        HPRC_Clock,
        HPRC_RocketVisual,
        HPRC_Hidden,
        HPRC_Viewer,
        HPRC_AIRBRAKES,
        HPRC_SERVO_STATUS,
        HPRC_PayloadMap,
        HPRC_PayloadAttitude,
        HPRC_PayloadBatteryVoltage
    };

    enum hprcDataType // TODO
    {
        HPRC_UTC,
        HPRC_MET,
        HPRC_Other,
    };

    bool imperial; // TODO
    bool msl; // TODO

    double m_max;
    double m_maxRecorded = 0;

    int m_filledPercent;
    int fillChanged();
    int getFilled();
    void setFilled(int input);

    QPoint m_mousePos;

    QString m_label;

    hprcWidgetType getType() const { return m_widgetType; }
    hprcDataType getDataType() const { return m_dataType; }

    void paintEvent(QPaintEvent *e);

public slots:

    void updateFilled(float input);
    void doSpeedTick(int input);

protected:

    hprcWidgetType m_widgetType;
    hprcDataType m_dataType;

signals:

};

class hprcTimeline : public hprcDisplayWidget
{
    Q_OBJECT
public:
    explicit hprcTimeline(QWidget *parent = nullptr);

signals:
};

class hprcGauge : public hprcDisplayWidget
{
    Q_OBJECT
public:


    explicit hprcGauge(QWidget *parent = nullptr);

signals:
};

class hprcAltitudeGauge : public hprcGauge
{


public:

    explicit hprcAltitudeGauge(QWidget *parent = nullptr);

};

class hprcVelocityGauge : public hprcGauge
{


public:

    explicit hprcVelocityGauge(QWidget *parent = nullptr);

};

class hprcAccelerationGauge : public hprcGauge
{


public:

    explicit hprcAccelerationGauge(QWidget *parent = nullptr);

};

class hprcAttitudeWidget: public hprcDisplayWidget
{

public:

    explicit hprcAttitudeWidget(QWidget *parent = nullptr);
    void mouseMoveEvent(QMouseEvent *e);
};

class hprcGraph : public hprcDisplayWidget
{
public:

    explicit hprcGraph(QWidget *parent = nullptr);
    void mouseMoveEvent(QMouseEvent *e);
};

class hprcPayloadGraph : public hprcDisplayWidget
{
Q_OBJECT

public:
    explicit hprcPayloadGraph(QWidget *parent = nullptr);
    void mouseMoveEvent(QMouseEvent *e);

    QList<MainWindow::graphPoint> verticalSpeedData;
    static const int MAX_RENDERED_POINTS = 50;

    QTransform transform;
    QPolygonF graphPolygonAltitude;
    QPolygonF graphPolygonVerticalSpeed;
    int startIndexAltitude;
    int startIndexVerticalSpeed;

    //DEBUG; REMOVE THIS WHEN NOT IN USE
    std::chrono::time_point<std::chrono::high_resolution_clock> lastTime = std::chrono::high_resolution_clock::now();
    int fps = 0;
    int frames = 0;

private:
    MainWindow* mainWindow;

private slots:
    void updateVerticalSpeed();
};

class hprcAlarmPanel : public hprcDisplayWidget
{
public:

    explicit hprcAlarmPanel(QWidget *parent = nullptr);
};

class hprcClock : public hprcDisplayWidget
{
public:

    explicit hprcClock(QWidget *parent = nullptr);
};
class hprcUTCClock : public hprcClock
{
public:

    explicit hprcUTCClock(QWidget *parent = nullptr) : hprcClock{parent} { m_dataType = HPRC_UTC; }
};
class hprcMETClock : public hprcClock
{
public:

    explicit hprcMETClock(QWidget *parent = nullptr) : hprcClock{parent} { m_dataType = HPRC_MET; }
};

class hprcAirbrakes : public hprcDisplayWidget {
public:
    explicit hprcAirbrakes(QWidget* parent = nullptr);
};
class hprcRocketVisualizer : public hprcDisplayWidget
{
public:

    explicit hprcRocketVisualizer(QWidget *parent = nullptr);
};

class hprcViewer : public hprcDisplayWidget
{
public:
    explicit hprcViewer(QWidget *parent = nullptr);
    void orientRocket(QQuaternion orientation) const;
    void updateColors(QColor panel, QColor highlight) const;

    float m_rocketVertOffset = -1.8f;
    float m_cameraDistToRocket = 40.0f;
    float m_rocketScale = 0.18f;

    // 3D Props
    Qt3DCore::QTransform *m_rocketTransform;
    Qt3DExtras::Qt3DWindow *m_view;
    Qt3DExtras::QDiffuseSpecularMaterial *m_rocketMaterial;

    // Note: the -90 is there to orient the rocket correctly
    // since the STL will face out of the screen by default
    // Note: euler angles are pitch, yaw, roll
    QQuaternion m_rocketOrientVertically = QQuaternion::fromEulerAngles(-90.0f, 0.0f, 0.0f);
};

class hprcServoStatusWidget: public hprcDisplayWidget
{
public:
    explicit hprcServoStatusWidget(QWidget *parent = nullptr);
};

class hprcPayloadMap : public hprcDisplayWidget
{
public:
    explicit hprcPayloadMap(QWidget *parent = nullptr);

    // Store a reference to the map image here for rendering
    QImage *m_mapImage;

    inline static double longPerPixel = 0.000303399269104;
    inline static double latPerPixel = 0.000251667206953;

    QPointF centerGlobalPoint = QPointF(-106.97552837089243, 32.98990645338422); // Long, lat

    /* Returns a point relative to the center of the widget the corresponds to the GPS coordinate. */
    static QPoint calculateWidgetPoint(QPointF centerPoint, QPointF globalPoint, double widgetScalingFactor);
};

class hprcPayloadBatteryVoltage : public hprcDisplayWidget
{
public:
    explicit hprcPayloadBatteryVoltage(QWidget *parent = nullptr);
};

class hprcRollGauge : public hprcGauge
{


public:

    explicit hprcRollGauge(QWidget *parent = nullptr);

};

class hprcPitchGauge : public hprcGauge
{


public:

    explicit hprcPitchGauge(QWidget *parent = nullptr);

};

#endif // HPRCDISPLAYWIDGET_H
