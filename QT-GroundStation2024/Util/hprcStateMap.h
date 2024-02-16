#ifndef HPRCSTATEMAP_H
#define HPRCSTATEMAP_H

#include <map>
#include "../stylepainters.h"

class hprcStateMaps
{
public:
    // data maps
    static std::map<double, QString> stateMap;
    static std::map<int, int> dataMap1;
    static std::map<int, int> dataMap2;
    static std::map<int, int> dataMap3;
    static std::map<HPRCStyle::HPRCAlarmType, bool> alarmMapW;
    static std::map<HPRCStyle::HPRCAlarmType, bool> alarmMapA;

    static void makeMaps()
    {

        double ticks[5] = {1, 0.85, 0.5, 0.25, 0}; // these need to be ordered bottom to top.  the system draws from top-left so "1" is the bottom (top - 1 * height)
        QString states[4] = {"BOOST", "CRUISE", "DROGUE", "MAIN"}; // will be associated with the space between [n] and [n+1] eg BOOST is between 1 and 0.85
        for(int index = 0; index < (sizeof(ticks)/sizeof(ticks[0])) - 1; index++)
        {
            QString value = QString::asprintf("-%d", index);
            double start = ticks[index];
            double end = ticks[index + 1];
            double middle = (start - end) / 2.0 + end;
            stateMap.insert(std::make_pair(start, value));
            stateMap.insert(std::make_pair(middle, states[index]));
        }
        stateMap.insert(std::make_pair(ticks[sizeof(ticks)/sizeof(ticks[0]) - 1], QString("-")));

        alarmMapA.insert(std::make_pair(HPRCStyle::ALARM_MasterAbort, true));
        alarmMapA.insert(std::make_pair(HPRCStyle::ALARM_Pitch, true));

        alarmMapW.insert(std::make_pair(HPRCStyle::ALARM_MasterWarn, true));
        alarmMapW.insert(std::make_pair(HPRCStyle::ALARM_PowerLoss, false));
        alarmMapW.insert(std::make_pair(HPRCStyle::ALARM_LowPower, false));
        alarmMapW.insert(std::make_pair(HPRCStyle::ALARM_SignalLoss, true));
        alarmMapW.insert(std::make_pair(HPRCStyle::ALARM_EarlyChute, true));
        alarmMapW.insert(std::make_pair(HPRCStyle::ALARM_Ballistic, false));
        alarmMapW.insert(std::make_pair(HPRCStyle::ALARM_MainDeployFail, false));
    }
};

#endif // HPRCSTATEMAP_H
