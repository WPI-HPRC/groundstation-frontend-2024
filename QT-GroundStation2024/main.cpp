#include "mainwindow.h"
#include "rocketwindow.h"
#include "styles.h"
#include <iostream>

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyle(new HPRCDarkStyle(nullptr));
    MainWindow w;
    RocketWindow r;
    a.setStyle(new HPRCDarkStyle(w.getCurrentData()));
    w.centralWidget()->setBackgroundRole(QPalette::Window);
    r.centralWidget()->setBackgroundRole(QPalette::Window);
    w.show();
//    r.show();
    w.showMaximized();
//    r.showMaximized();
    w.update();
    r.update();

    return a.exec();
}
