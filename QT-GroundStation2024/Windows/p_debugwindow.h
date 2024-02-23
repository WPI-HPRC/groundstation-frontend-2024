#ifndef P_DEBUGWINDOW_H
#define P_DEBUGWINDOW_H

#include <QMainWindow>

namespace Ui {
class p_DebugWindow;
}

class p_DebugWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit p_DebugWindow(QWidget *parent = nullptr);
    ~p_DebugWindow();

private:
    Ui::p_DebugWindow *ui;
};

#endif // P_DEBUGWINDOW_H
