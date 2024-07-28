#ifndef ROCKETWINDOW_H
#define ROCKETWINDOW_H

#include <QMainWindow>

namespace Ui {
class RocketWindow;
}

class RocketWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit RocketWindow(QWidget *parent = nullptr);
    ~RocketWindow();

private:
    Ui::RocketWindow *ui;
};

#endif // ROCKETWINDOW_H
