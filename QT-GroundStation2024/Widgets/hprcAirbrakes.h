#include "hprcdisplaywidget.h"

#ifndef AIRBRAKESWIDGET_H
#define AIRBRAKESWIDGET_H

class hprcAirbrakes : public hprcDisplayWidget {
public:
    explicit hprcAirbrakes(QWidget* parent = nullptr);

    //void drawDetailedView(QPainter* p, hprcDisplayWidget* w, DrawResources* drawResources, MainWindow::dataPoint* m_latest) override;
};

#endif