#include "Windows/mainwindow.h"
#include "Windows/rocketwindow.h"
#include "Windows/payloadwindow.h"
#include "Windows/debugwindow.h"

#include "styles.h"
#include <iostream>

#include <QApplication>
#include "Util/hprcStateMap.h"

int main(int argc, char *argv[])
{
    hprcStateMaps::makeMaps();

    qputenv("QT3D_RENDERER", "opengl");
    QApplication application(argc, argv);
    application.setStyle(new HPRCDarkStyle(nullptr));

    MainWindow mainWindow;
    RocketWindow rocketWindow;
    PayloadWindow payloadWindow;
    DebugWindow debugWindow;

    application.setStyle(new HPRCDarkStyle(mainWindow.getCurrentData()));

    mainWindow.centralWidget()->setBackgroundRole(QPalette::Window);
    rocketWindow.centralWidget()->setBackgroundRole(QPalette::Window);
    payloadWindow.centralWidget()->setBackgroundRole(QPalette::Window);
    debugWindow.centralWidget()->setBackgroundRole(QPalette::Window);

    debugWindow.showMaximized();
    mainWindow.showMaximized();
    rocketWindow.showMaximized();
    payloadWindow.showMaximized();

    mainWindow.update();
    rocketWindow.update();
    payloadWindow.update();
    debugWindow.update();

    int code = application.exec();

    mainWindow.m_websocket->close();

    return code;
}
