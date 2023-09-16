#ifndef HPRCDISPLAYWIDGET_H
#define HPRCDISPLAYWIDGET_H

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
        HPRC_Graph,
        HPRC_Alarm,
        HPRC_Clock,
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

    QString m_label;

    hprcWidgetType getType() const { return m_widgetType; }
    hprcDataType getDataType() const { return m_dataType; }

    void paintEvent(QPaintEvent *e);

public slots:

    void updateFilled(int input);

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

class hprcGraph : public hprcDisplayWidget
{
public:

    explicit hprcGraph(QWidget *parent = nullptr);
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


#endif // HPRCDISPLAYWIDGET_H
