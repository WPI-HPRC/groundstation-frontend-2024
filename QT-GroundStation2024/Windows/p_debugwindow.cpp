#include "p_debugwindow.h"
#include "qstyle.h"
#include "ui_p_debugwindow.h"

p_DebugWindow::p_DebugWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::p_DebugWindow)
{
    ui->setupUi(this);
    setPalette(QApplication::style()->standardPalette());
    setBackgroundRole(QPalette::Window);
}

p_DebugWindow::~p_DebugWindow()
{
    delete ui;
}
