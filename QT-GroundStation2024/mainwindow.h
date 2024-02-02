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
#include <QQuaternion>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    enum RocketState {
        PRE_LAUNCH, LAUNCH, COAST, DROGUE_DESCENT, MAIN_DESCENT, RECOVERY
    };

    struct graphPoint {
        float value = 0;
        float time = 0;
    };

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
        QList<graphPoint> accData;
        QList<graphPoint> velData;
        QList<graphPoint> altData;
        QQuaternion orientation;
        float gyroX = 0;
        float gyroY = 0;
        float gyroZ = 0;
        float payloadServo1Position;
        float payloadServo2Position;
        float desiredPayloadServo1Position;
        float desiredPayloadServo2Position;
        float payloadBatteryVoltage;
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

    QWebSocket m_webSocket;

    dataPoint* getCurrentData() { return &m_currentData; }

    int dataDeletionTime;

signals:
    void accUpdated(int);
    void velUpdated(int);
    void altUpdated(float, float);
    void stateUpdated(int);
    void rocketTimeUpdated(float);
    void groundTimeUpdated();
    void orientationUpdated();
    void gyroUpdated();
    void currentAirbrakesUpdated(float);
    void desiredAirbrakesUpdated(float);
    void tick();
    void speedTick(int);
    void payloadServo1PositionUpdated(float);
    void payloadServo2PositionUpdated(float);
    void desiredPayloadServo1PositionUpdated(float);
    void desiredPayloadServo2PositionUpdated(float);

public slots:
    void update();
    void onConnected();
    void onDisconnected();
    void onTextMessageReceived(QString message);

private:
    Ui::MainWindow *ui;



};
#endif // MAINWINDOW_H
