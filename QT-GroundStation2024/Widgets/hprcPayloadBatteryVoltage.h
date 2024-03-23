#include "./hprcdisplaywidget.h"

class hprcPayloadBatteryVoltage : public hprcDisplayWidget
{
public:
    explicit hprcPayloadBatteryVoltage(QWidget* parent = nullptr);

    static constexpr float MIN_VOLTAGE = 5.0;
    static constexpr float MAX_VOLTAGE = 8.4;
    static constexpr float LOW_THRESHOLD = 6.5;
};