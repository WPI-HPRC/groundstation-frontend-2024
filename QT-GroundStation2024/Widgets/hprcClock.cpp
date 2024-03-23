#include "hprcClock.h"

#include "Windows/mainwindow.h"

#include <QApplication>

hprcClock::hprcClock(QWidget* parent) :
    hprcDisplayWidget(parent)
{
    // subscribe to any data that needs to cause an update
    foreach(QWidget * w, qApp->topLevelWidgets())
        if (MainWindow* mainWin = qobject_cast<MainWindow*>(w))
        {
            connect(mainWin, SIGNAL(tick()), this, SLOT(repaint()));
        }


    m_widgetType = HPRC_Clock;
}