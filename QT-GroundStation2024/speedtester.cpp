#include "speedtester.h"
#include "mainwindow.h"
#include <QObject>
#include <QWidget>

#define NUM_DURATION_FOR_AVERAGE 200

#include <iostream>
#include <iomanip>

SpeedTester::SpeedTester()
{
    foreach (QWidget *w, qApp->topLevelWidgets())
        if (MainWindow* mainWin = qobject_cast<MainWindow*>(w))
        {
            connect(mainWin, SIGNAL(speedTick(int)), this, SLOT(tickOccurred(int)));
        }
}

void SpeedTester::tickOccurred(int _)
{
    std::chrono::high_resolution_clock::time_point t_now = std::chrono::high_resolution_clock::now();

    double deltaTime_ms = std::chrono::duration<double, std::milli>(t_now-t_last).count();

    durations.push_back(deltaTime_ms);

    numTicks++;

    if(numTicks % NUM_DURATION_FOR_AVERAGE == 0)
    {
        double tickAverage = std::accumulate(durations.begin(), durations.end(), 0.0) / durations.size();
        durations.clear();

        std::cout << "For last " << NUM_DURATION_FOR_AVERAGE << " ticks:\n\t"
                  << "Time between ticks: " << std::setprecision(3) << tickAverage << "ms\n\t"
                  << "Target: " << TIMER_TICK_MS << "ms\n\t"
//                  << ((average - TIMER_TICK_MS) > 2.5 ? "\033[1;31m" : "\033[1;32m") // Set the color
                  << "Difference: " << (tickAverage - TIMER_TICK_MS) << "ms\033[0m\n\t"
                  << "Estimated minimum FPS: "<< std::setprecision(3) << (1000/(tickAverage))
                  << "\n" << std::endl;

    }
    t_last = std::chrono::high_resolution_clock::now();
}

