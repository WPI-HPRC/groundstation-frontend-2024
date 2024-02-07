#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define RUN_SPEED_TESTS false
#define TIMER_TICK_MS 1

#if RUN_SPEED_TESTS
    #include "speedtester.h"
#endif
#include "qdatetime.h"
#include "qwebsocket.h"
#include <QMainWindow>
#include <QWebSocketServer>
#include <QAbstractSocket>
#include <QQuaternion>
#include "hprcCircularBuffer.h"
#include "hprcwebsocket.h"


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
        float rocketTimeSinceLaunch = 0;
        int groundTime = 0;
        float desiredAirbrakes = 0;
        float currentAirbrakes = 0;
        GraphPointCircularBuffer *accData;
        GraphPointCircularBuffer *velData;
        GraphPointCircularBuffer *altData;
        QQuaternion orientation;
        float gyroX = 0;
        float gyroY = 0;
        float gyroZ = 0;
        bool timelineActivated[5] = {false, false, false, false, false};
        QString timelineTimes[5] = {"00:00:00", "00:00:00", "00:00:00", "00:00:00", "00:00:00"};
    };

#if RUN_SPEED_TESTS
    SpeedTester speedTester = SpeedTester();
#endif

    QDateTime m_groundLaunchTime;
    float m_rocketLaunchTime;

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void updateData(dataPoint p);
    int pts = 0;

    dataPoint m_currentData;
    dataPoint m_dataBuffer;

    HPRCWebSocket* m_websocket;

    dataPoint* getCurrentData() { return &m_currentData; }

signals:
    void accUpdated(int);
    void velUpdated(int);
    void altUpdated(int);
    void stateUpdated(int);
    void rocketTimeUpdated(float);
    void groundTimeUpdated();
    void orientationUpdated();
    void gyroUpdated();
    void currentAirbrakesUpdated(float);
    void desiredAirbrakesUpdated(float);
    void tick();
    void speedTick(int);

public slots:
    void update();
    void onTextMessageReceived(QString message);

private:
    Ui::MainWindow *ui;



};
#endif // MAINWINDOW_H
