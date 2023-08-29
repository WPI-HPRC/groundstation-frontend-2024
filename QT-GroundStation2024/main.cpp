#include "mainwindow.h"
#include "styles.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyle(new HPRCDarkStyle);
    MainWindow w;
    w.centralWidget()->setBackgroundRole(QPalette::Window);
    w.show();
    w.showMaximized();

    return a.exec();
}
