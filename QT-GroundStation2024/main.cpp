#include "mainwindow.h"
#include "rocketwindow.h"
#include "styles.h"
#include "hprcCircularBuffer.h"
#include <iostream>

#include <QApplication>

int main(int argc, char *argv[])
{

    GraphPointCircularBuffer *buffer = createGraphPointCirularBuffer(10);

    for (int i = 0; i < 7; i++) {
        graphPointCirularBufferAdd(buffer, MainWindow::graphPoint {
                                                                  .value = (float)i,
                                                                  .time = (float)5 * i
                                           });
    }

    MainWindow::graphPoint *graphPoint;

    for (int i = 0; i < 10; i++) {
        std::cout << graphPointCircularBufferGetValueAtIndex(buffer, i).value << "\n";
    }

    MainWindow::graphPoint gp = graphPointCircularBufferGetValueAtIndex(buffer, 1);

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
