#ifndef SPEEDTESTER_H
#define SPEEDTESTER_H

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

public slots:

    void tickOccurred();

signals:
};

#endif // SPEEDTESTER_H
