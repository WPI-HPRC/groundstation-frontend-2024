#include "speedtester.h"
#include "mainwindow.h"
#include <QObject>
#include <QWidget>

#define NUM_DURATION_FOR_AVERAGE 200
#define TIME_INDIVIDUAL_WIDGETS false

#define RUN_SINGLE_TEST true
#define SINGULAR_WIDGET_TEST_NAME "Graphs"

#include <iostream>
#include <iomanip>

bool SpeedTester::compareDurations(SpeedTester_TimeDuration t1, SpeedTester_TimeDuration t2)
{
    return t1.duration > t2.duration;
}

void SpeedTester::runSpeedTests()
{
    std::vector<SpeedTester_TimeDuration> windowTimeDurations;

#if !RUN_SINGLE_TEST
    std::cout << "Running speed tests for windows...\n\n" << std::endl;
    foreach (QWidget *w, qApp->topLevelWidgets())
    {
        std::vector<double> windowDurations;

        for (int i = 0; i < NUM_DURATION_FOR_AVERAGE * 5; i++)
        {
            std::chrono::high_resolution_clock::time_point t_before = std::chrono::high_resolution_clock::now();

            w->repaint();

            std::chrono::high_resolution_clock::time_point t_now = std::chrono::high_resolution_clock::now();

            double deltaTime_ms = std::chrono::duration<double, std::milli>(t_now-t_before).count();
            windowDurations.push_back(deltaTime_ms);
        }
        double avg = std::accumulate(windowDurations.begin(), windowDurations.end(), 0.0) / windowDurations.size();

        windowTimeDurations.push_back(SpeedTester_TimeDuration{avg, w->objectName().toStdString()});
    }

    std::sort(windowTimeDurations.begin(), windowTimeDurations.end(), compareDurations);

    for(SpeedTester_TimeDuration& duration : windowTimeDurations)
    {
        std::cout << "Speed test for window " << duration.name << std::endl;
        std::cout << "\tAverage draw time: " << std::setprecision(4) << duration.duration << "ms" << std::endl;
        std::cout << "\tEstimated fps: "  << 1000/duration.duration << "\n" << std::endl;
    }

    std::cout << "\nRunning speed test for widgets...\n" << std::endl;
#endif

    QList<QWidget*> allWidgets = QApplication::allWidgets();
    std::vector<double> averages;
    std::vector<SpeedTester_TimeDuration> widgetTimeDurations;
    for(QWidget* w : allWidgets)
    {
        if((w->findChildren<QWidget*>().length() != 0 && w->objectName() != QString("Graphs"))
            || w->parent()->objectName() == QString("Graphs")) // QGraphicsView is weird
            continue;

#if RUN_SINGLE_TEST
        if (QString::compare(w->objectName(), SINGULAR_WIDGET_TEST_NAME) != 0)
            continue;
#endif

        std::vector<double> widgetDurations;

        for (int i = 0; i < NUM_DURATION_FOR_AVERAGE * 5; i++)
        {
            std::chrono::high_resolution_clock::time_point t_before = std::chrono::high_resolution_clock::now();

            w->repaint();

            std::chrono::high_resolution_clock::time_point t_now = std::chrono::high_resolution_clock::now();

            double deltaTime_ms = std::chrono::duration<double, std::milli>(t_now-t_before).count();
            widgetDurations.push_back(deltaTime_ms);
        }
        double avg = std::accumulate(widgetDurations.begin(), widgetDurations.end(), 0.0) / widgetDurations.size();

        averages.push_back(avg);
        widgetTimeDurations.push_back(SpeedTester_TimeDuration {avg, w->objectName().toStdString()});
    }

    std::sort(widgetTimeDurations.begin(), widgetTimeDurations.end(), compareDurations);

    double total = std::accumulate(averages.begin(), averages.end(), 0.0);

    for(SpeedTester_TimeDuration& duration : widgetTimeDurations)
    {

#if RUN_SINGLE_TEST
        std::cout << "\nTime for " << duration.name << ": " << duration.duration << "ms" << std::endl;
#else
        std::cout << "\t" << std::setprecision(4) << duration.duration / total * 100 << "% " << duration.name << ", " << duration.duration << "ms" << std::endl;
#endif
    }

#if !RUN_SINGLE_TEST
    std::cout << "\nEstimated total time: " << total << "ms" << std::endl;
    std::cout << "Estimated fps: " << (1000/total) << "\n\n\n" << std::endl;
#endif
}

SpeedTester::SpeedTester()
{
    foreach (QWidget *w, qApp->topLevelWidgets())
    {
        if (MainWindow* mainWin = qobject_cast<MainWindow*>(w))
        {
            connect(mainWin, SIGNAL(speedTick(int)), this, SLOT(tickOccurred(int)));
        }
    }
}

void SpeedTester::tickOccurred(int _)
{
    std::chrono::high_resolution_clock::time_point t_now = std::chrono::high_resolution_clock::now();

    numTicks++;

    if(numTicks == 30) // Wait for the window to open and for things to settle
    {
#if TIME_INDIVIDUAL_WIDGETS
        runSpeedTests();
#endif
    }
    else if(numTicks < 30)
    {
        return;
    }

    double deltaTime_ms = std::chrono::duration<double, std::milli>(t_now-t_last).count();

    durations.push_back(deltaTime_ms);


    if(numTicks % NUM_DURATION_FOR_AVERAGE == 0)
    {
        double tickAverage = std::accumulate(durations.begin(), durations.end(), 0.0) / durations.size();
        durations.clear();

        std::cout << "For last " << NUM_DURATION_FOR_AVERAGE << " ticks:\n\t"
                  << "Time between ticks: " << std::setprecision(4) << tickAverage << "ms\n\t"
                  << "Target: " << TIMER_TICK_MS << "ms\n\t"
//                  << ((average - TIMER_TICK_MS) > 2.5 ? "\033[1;31m" : "\033[1;32m") // Set the color
                  << "Difference: " << (tickAverage - TIMER_TICK_MS) << "ms\033[0m\n\t"
                  << "Estimated fps: " << (1000/(tickAverage))
                  << "\n" << std::endl;

    }
    t_last = std::chrono::high_resolution_clock::now();
}

