#include "Windows/mainwindow.h"
#include "Windows/rocketwindow.h"
#include "Windows/payloadwindow.h"
#include "Windows/debugwindow.h"
#include "Windows/p_debugwindow.h"

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
//    PayloadWindow payloadWindow;
//    DebugWindow debugWindow;
//    p_DebugWindow p_debugWindow;


    application.setStyle(new HPRCDarkStyle(mainWindow.getCurrentData()));

    mainWindow.centralWidget()->setBackgroundRole(QPalette::Window);
    rocketWindow.centralWidget()->setBackgroundRole(QPalette::Window);
//    payloadWindow.centralWidget()->setBackgroundRole(QPalette::Window);
//    debugWindow.centralWidget()->setBackgroundRole(QPalette::Window);
//    p_debugWindow.centralWidget()->setBackgroundRole(QPalette::Window);

    mainWindow.showMaximized();
    rocketWindow.showMaximized();
    //    payloadWindow.showMaximized();
//    p_debugWindow.showMaximized();
//    debugWindow.showMaximized();

    mainWindow.update();
    rocketWindow.update();
//    payloadWindow.update();
//    debugWindow.update();
//    p_debugWindow.update();

    int code = application.exec();

    mainWindow.m_websocket->close();

    return code;
}
