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

    struct DrawResources {
        QBrush m_backgroundBrush;
        QBrush m_panelBrush;
        QBrush m_textBrush;
        QBrush m_highlightBrush;
        QBrush m_buttonBrush;
        QBrush m_transparentBrush;

        QFont m_widgetFancy;
        QFont m_widgetLarge;
        QFont m_widgetMedium;
        QFont m_widgetSmall;
    };

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
        HPRC_PayloadBatteryVoltage,
        HPRC_GpsLock,
        HPRC_DebugWidget,
        HPRC_p_DebugWidget,
        HPRC_serverConnection,
        HPRC_DETAILED_VIEW_WIDGET,
        HPRC_GRAPHICS_DETAILED_VIEW_WIDGET
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

    // Detailed View Config
    int detailedViewWindowWidth = 500;
    int detailedViewWindowHeight = 500;

    hprcWidgetType getType() const { return m_widgetType; }
    hprcDataType getDataType() const { return m_dataType; }

    void paintEvent(QPaintEvent *e);

    //Can be overwritten to draw custom detailed view
    //virtual void drawDetailedView(QPainter* p, hprcDisplayWidget* w, DrawResources* drawResources, MainWindow::dataPoint* m_latest) {}

public slots:

    void updateFilled(int input);
    void updateFilled(float input);
    void doSpeedTick(int input);

protected:

    hprcWidgetType m_widgetType;
    hprcDataType m_dataType;
    bool detailedViewEnabled = false;

private:
    void makeDetailedWidget(hprcDisplayWidget *baseWidget);

    void mousePressEvent(QMouseEvent *event) override;

signals:

};

#endif // HPRCDISPLAYWIDGET_H
