#include "./hprcdisplaywidget.h"

#include "Windows/mainwindow.h"

class hprcPayloadGraph : public hprcDisplayWidget
{
    Q_OBJECT

public:
    explicit hprcPayloadGraph(QWidget* parent = nullptr);
    void mouseMoveEvent(QMouseEvent* e);

    QList<MainWindow::graphPoint> verticalSpeedData;
    static const int MAX_RENDERED_POINTS = 9999999;

    QTransform transform;
    QPolygonF graphPolygonAltitude;
    QPolygonF graphPolygonVerticalSpeed;
    int startIndexAltitude;
    int startIndexVerticalSpeed;

    /*DEBUG; REMOVE THIS WHEN NOT IN USE
    std::chrono::time_point<std::chrono::high_resolution_clock> lastTime = std::chrono::high_resolution_clock::now();
    int fps = 0;
    int frames = 0;
    */

private:
    MainWindow* mainWindow;

private slots:
    void updateVerticalSpeed();
};