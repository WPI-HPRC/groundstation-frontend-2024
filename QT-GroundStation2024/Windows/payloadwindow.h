#ifndef PAYLOADWINDOW_H
#define PAYLOADWINDOW_H

#include <QMainWindow>

namespace Ui {
class PayloadWindow;
}

class PayloadWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PayloadWindow(QWidget *parent = nullptr);
    ~PayloadWindow();

private:
    Ui::PayloadWindow *ui;
};

#endif // PAYLOADWINDOW_H
