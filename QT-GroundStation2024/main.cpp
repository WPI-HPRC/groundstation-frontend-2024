#include "mainwindow.h"
#include "styles.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyle(new HPRCLightStyle(nullptr));
    MainWindow w;
    a.setStyle(new HPRCLightStyle(w.getCurrentData()));
    w.centralWidget()->setBackgroundRole(QPalette::Window);
    w.show();
    w.showMaximized();
    w.update();

    return a.exec();
}
