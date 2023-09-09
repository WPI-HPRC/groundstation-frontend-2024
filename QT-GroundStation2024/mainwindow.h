#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    struct dataPoint {
        float acceleration = 0;
        float velocity = 0;
        float altitude = 0;
        int state = 0;
        float rocketTime = 0;
        float utcTime = 0;
    };

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void updateData(dataPoint p);

    dataPoint m_currentData;

    dataPoint* getCurrentData() { return &m_currentData; }

signals:
    void accUpdated(float);
    void velUpdated(float);
    void altUpdated(float);
    void stateUpdated(int);
    void rocketTimeUpdated(float);
    void utcTimeUpdated();

public slots:
    void update();

private:
    Ui::MainWindow *ui;

};
#endif // MAINWINDOW_H
