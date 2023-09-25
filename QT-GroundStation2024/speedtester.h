#ifndef SPEEDTESTER_H
#define SPEEDTESTER_H

#include <QMainWindow>
#include <chrono>
#include <qapplication.h>
#include <qobject.h>

class SpeedTester : public QObject
{
    Q_OBJECT
public:
    explicit SpeedTester();

protected:
    ulong numTicks = 0;
    std::chrono::high_resolution_clock::time_point t_last = std::chrono::high_resolution_clock::now();
    std::vector<double> durations;
    std::vector<double> widgetDurations;


public slots:

    void tickOccurred(int _);

signals:

};

#endif // SPEEDTESTER_H
