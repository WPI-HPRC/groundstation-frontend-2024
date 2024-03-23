#include "hprcAirbrakes.h"

#include "Windows/mainwindow.h"

#include <QApplication>

hprcAirbrakes::hprcAirbrakes(QWidget* parent) :
    hprcDisplayWidget(parent)
{
    // subscribe to any data that needs to cause an update
    foreach(QWidget * w, qApp->topLevelWidgets())
        if (MainWindow* mainWin = qobject_cast<MainWindow*>(w))
        {
            connect(mainWin, SIGNAL(desiredAirbrakesUpdated(float)), this, SLOT(repaint()));
            connect(mainWin, SIGNAL(currentAirbrakesUpdated(float)), this, SLOT(repaint()));
        }


    m_widgetType = HPRC_AIRBRAKES;
}