#include "mainwindow.h"
#include "rocketwindow.h"
#include "payloadwindow.h"
#include "styles.h"

#include <QApplication>

int main(int argc, char *argv[])
{
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

    return a.exec();
}
