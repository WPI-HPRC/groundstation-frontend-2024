#include "./hprcdisplaywidget.h"

class hprcAttitudeWidget : public hprcDisplayWidget
{

public:

    explicit hprcAttitudeWidget(QWidget* parent = nullptr);
    void mouseMoveEvent(QMouseEvent* e);
};