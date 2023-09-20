#ifndef HPRCDISPLAYWIDGET_H
#define HPRCDISPLAYWIDGET_H

#include "mainwindow.h"
#include <QObject>
#include <QWidget>

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
        HPRC_Alarm,
        HPRC_Clock,
        HPRC_RocketVisual,
        HPRC_Hidden,
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

    void updateFilled(int input);
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

    float m_maxDegreeRange = 15;
    float m_degreeOffsetYaw = 0;
    float m_degreeOffsetPitch = 0;

    std::vector<std::vector<float>> circleLocationsDegrees {
                                                           std::vector<float> {
            3.0, 7.0, 12.0, 15.0
        }
    };
};

class hprcGraph : public hprcDisplayWidget
{
public:

    explicit hprcGraph(QWidget *parent = nullptr);
    void mouseMoveEvent(QMouseEvent *e);
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
class hprcRocketVisualizer : public hprcDisplayWidget
{
public:

    explicit hprcRocketVisualizer(QWidget *parent = nullptr);
};


#endif // HPRCDISPLAYWIDGET_H
