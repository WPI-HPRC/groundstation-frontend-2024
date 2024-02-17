#include "Windows/mainwindow.h"
#include "Windows/rocketwindow.h"
#include "Windows/payloadwindow.h"

#include "styles.h"
#include <iostream>

#include <QApplication>
#include "Util/hprcStateMap.h"

int main(int argc, char *argv[])
{
    hprcStateMaps::makeMaps();

    qputenv("QT3D_RENDERER", "opengl");
    QApplication a(argc, argv);
    a.setStyle(new HPRCDarkStyle(nullptr));
    MainWindow w;
    RocketWindow r;
    PayloadWindow p;
    a.setStyle(new HPRCDarkStyle(w.getCurrentData()));
    w.centralWidget()->setBackgroundRole(QPalette::Window);
    r.centralWidget()->setBackgroundRole(QPalette::Window);
    p.centralWidget()->setBackgroundRole(QPalette::Window);
    w.show();
    r.show();
    p.show();
    w.showMaximized();
    r.showMaximized();
    p.showMaximized();
    w.update();
    r.update();
    p.update();

    int code = a.exec();

    w.m_websocket->close();

    return code;
}
