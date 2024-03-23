#include "hprcServoStatusWidget.h"

#include "Windows/mainwindow.h"

#include <QApplication>

hprcServoStatusWidget::hprcServoStatusWidget(QWidget* parent) {
    // subscribe to any data that needs to cause an update
    foreach(QWidget * w, qApp->topLevelWidgets())
        if (MainWindow* mainWin = qobject_cast<MainWindow*>(w))
        {
            connect(mainWin, SIGNAL(payloadServo1PositionUpdated(float)), this, SLOT(repaint()));
            connect(mainWin, SIGNAL(desiredPayloadServo1PositionUpdated(float)), this, SLOT(repaint()));
        }
    m_widgetType = HPRC_SERVO_STATUS;
}