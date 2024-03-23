#include "./hprcdisplaywidget.h"

class hprcClock : public hprcDisplayWidget
{
public:

    explicit hprcClock(QWidget* parent = nullptr);
};
class hprcUTCClock : public hprcClock
{
public:

    explicit hprcUTCClock(QWidget* parent = nullptr) : hprcClock{ parent } { m_dataType = HPRC_UTC; }
};
class hprcMETClock : public hprcClock
{
public:

    explicit hprcMETClock(QWidget* parent = nullptr) : hprcClock{ parent } { m_dataType = HPRC_MET; }
};