#include "debugwindow.h"
#include "qstyle.h"
#include "ui_debugwindow.h"

DebugWindow::DebugWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::DebugWindow)
{
    ui->setupUi(this);
    setPalette(QApplication::style()->standardPalette());
    setBackgroundRole(QPalette::Window);
}

DebugWindow::~DebugWindow()
{
    delete ui;
}
