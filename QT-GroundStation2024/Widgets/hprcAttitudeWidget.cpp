#include "hprcAttitudeWidget.h"

#include "Windows/mainwindow.h"
#include <QMouseEvent>
#include <QApplication>

hprcAttitudeWidget::hprcAttitudeWidget(QWidget* parent) :
    hprcDisplayWidget(parent)
{
    //    setMouseTracking(true);
    m_widgetType = HPRC_Attitude;
    foreach(QWidget * w, qApp->topLevelWidgets())
        if (MainWindow* mainWin = qobject_cast<MainWindow*>(w))
        {
            connect(mainWin, SIGNAL(orientationUpdated()), this, SLOT(repaint()));
            connect(mainWin, SIGNAL(gyroUpdated()), this, SLOT(repaint()));
        }
}

void hprcAttitudeWidget::mouseMoveEvent(QMouseEvent* e)
{
    m_mousePos = e->pos();
}