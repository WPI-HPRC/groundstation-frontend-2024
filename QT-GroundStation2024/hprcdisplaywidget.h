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
        HPRC_Hidden,
    };

    enum hprcDataType // TODO
    {
        HPRC_Altitude,
        HPRC_Velocity,
        HPRC_Acceleration,
        HPRC_Other,
    };

    bool imperial; // TODO
    bool msl; // TODO

    int m_filledPercent;
    int fillChanged();
    int getFilled();
    void setFilled(int input);

    QString m_label;

    hprcWidgetType getType() const { return m_widgetType; };

    void paintEvent(QPaintEvent *e);

public slots:

    void updateFilled(int input);

protected:

    hprcWidgetType m_widgetType;

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

#endif // HPRCDISPLAYWIDGET_H
