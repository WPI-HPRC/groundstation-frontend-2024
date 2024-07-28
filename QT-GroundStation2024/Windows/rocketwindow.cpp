#include "rocketwindow.h"
#include "qstyle.h"
#include "ui_rocketwindow.h"

RocketWindow::RocketWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RocketWindow)
{
    ui->setupUi(this);
    setPalette(QApplication::style()->standardPalette());
    setBackgroundRole(QPalette::Window);
}

RocketWindow::~RocketWindow()
{
    delete ui;
}
