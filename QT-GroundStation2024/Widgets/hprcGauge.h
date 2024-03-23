#include "./hprcdisplaywidget.h"

class hprcGauge : public hprcDisplayWidget
{
    Q_OBJECT
public:


    explicit hprcGauge(QWidget* parent = nullptr);

    void drawDetailedView(QPainter* p) override;

signals:
};

class hprcAltitudeGauge : public hprcGauge
{


public:

    explicit hprcAltitudeGauge(QWidget* parent = nullptr);

};

class hprcVelocityGauge : public hprcGauge
{


public:

    explicit hprcVelocityGauge(QWidget* parent = nullptr);

};

class hprcAccelerationGauge : public hprcGauge
{


public:

    explicit hprcAccelerationGauge(QWidget* parent = nullptr);

};

class hprcRollGauge : public hprcGauge
{


public:

    explicit hprcRollGauge(QWidget* parent = nullptr);

};

class hprcPitchGauge : public hprcGauge
{


public:

    explicit hprcPitchGauge(QWidget* parent = nullptr);

};