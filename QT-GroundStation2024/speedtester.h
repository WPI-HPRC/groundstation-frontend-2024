#pragma once

#include <QMainWindow>
#include <chrono>
#include <qapplication.h>
#include <qobject.h>

class SpeedTester : public QObject
{

    struct SpeedTester_TimeDuration {
        double duration;
        std::string name;
    };

    Q_OBJECT
public:
    explicit SpeedTester();

protected:
    ulong numTicks = 0;
    std::chrono::high_resolution_clock::time_point t_last = std::chrono::high_resolution_clock::now();
    std::vector<double> durations;
    std::vector<double> widgetDurations;
    bool hasLoadedWidgets = false;

private:
    void runSpeedTests();
    static bool compareDurations(SpeedTester_TimeDuration t1, SpeedTester_TimeDuration t2);


public slots:

    void tickOccurred(int _);

signals:

};
