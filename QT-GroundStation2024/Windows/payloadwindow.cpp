#include "payloadwindow.h"
#include "ui_payloadwindow.h"
#include "qstyle.h"

PayloadWindow::PayloadWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::PayloadWindow)
{
    ui->setupUi(this);
    setPalette(QApplication::style()->standardPalette());
    setBackgroundRole(QPalette::Window);
}

PayloadWindow::~PayloadWindow()
{
    delete ui;
}
