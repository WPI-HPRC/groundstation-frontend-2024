#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "qstyle.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setBackgroundRole(QPalette::Window);
    setPalette(QApplication::style()->standardPalette());
//    setupCentralWidget();
}

MainWindow::~MainWindow()
{
    delete ui;
}
