#include "Windows/mainwindow.h"
#include "Windows/rocketwindow.h"
#include "styles.h"
#include <iostream>

#include <QApplication>

int main(int argc, char *argv[])
{
    qputenv("QT3D_RENDERER", "opengl");
    QApplication a(argc, argv);
    a.setStyle(new HPRCDarkStyle(nullptr));
    MainWindow w;
    RocketWindow r;
    a.setStyle(new HPRCDarkStyle(w.getCurrentData()));
    w.centralWidget()->setBackgroundRole(QPalette::Window);
    r.centralWidget()->setBackgroundRole(QPalette::Window);
    w.show();
    r.show();
    w.showMaximized();
    r.showMaximized();
    w.update();
    r.update();

    int code = a.exec();

    w.m_websocket->close();

    return code;
}
