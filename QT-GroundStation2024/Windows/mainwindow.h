#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define RUN_SPEED_TESTS false
#define TIMER_TICK_MS 1

#if RUN_SPEED_TESTS
    #include "Util/speedtester.h"
#endif
#include "qdatetime.h"
#include "qwebsocket.h"
#include <QMainWindow>
#include <QWebSocketServer>
#include <QAbstractSocket>
#include <QQuaternion>
#include "Util/hprcCircularBuffer.h"
#include "Util/hprcwebsocket.h"


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
        GraphPointCircularBuffer *accData;
        GraphPointCircularBuffer *velData;
        GraphPointCircularBuffer *altData;

        QList<graphPoint> accData1;
        QList<graphPoint> velData1;
        QList<graphPoint> altData1;


        QQuaternion orientation;
        float gyroX = 0;
        float gyroY = 0;
        float gyroZ = 0;
        float payloadServo1Position;
        float payloadServo2Position;
        float desiredPayloadServo1Position;
        float desiredPayloadServo2Position;
        float payloadBatteryVoltage;
        bool timelineActivated[5] = {false, false, false, false, false};
        QString timelineTimes[5] = {"00:00:00", "00:00:00", "00:00:00", "00:00:00", "00:00:00"};
        float p_gpsLat;
        float p_gpsLong;
        float p_targetGpsLat;
        float p_targetGpsLong;

        float i, j, k, w;

        bool gpsLock;
        int numSatellites;

        int epochTime;

        float pressure;
    };

    QMap<QString, std::function<void(dataPoint&, const QString&)>> elementMap = {
                                                                                   {"accelZ", [](dataPoint& dp, const QString& value) { dp.acceleration = value.toFloat(); }},
                                                                                   {"velocity", [](dataPoint& dp, const QString& value) { dp.velocity = value.toFloat(); }},
                                                                                   {"altitude", [](dataPoint& dp, const QString& value) { dp.altitude = value.toFloat(); }},
                                                                                   {"state", [](dataPoint& dp, const QString& value) { dp.state = value.toInt(); }},
                                                                                   {"timestamp", [](dataPoint& dp, const QString& value) { dp.rocketTime = value.toFloat(); }},
                                                                                   {"airbrakesDeploy", [](dataPoint& dp, const QString& value) { dp.currentAirbrakes = value.toFloat(); dp.desiredAirbrakes = dp.currentAirbrakes; }},
                                                                                   {"i", [](dataPoint& dp, const QString& value) { dp.i = value.toFloat(); dp.orientation.setX(value.toFloat()); }},
                                                                                   {"k", [](dataPoint& dp, const QString& value) { dp.k = value.toFloat(); dp.orientation.setY(value.toFloat()); }},
                                                                                   {"j", [](dataPoint& dp, const QString& value) { dp.j = value.toFloat(); dp.orientation.setZ(value.toFloat()); }},
                                                                                   {"w", [](dataPoint& dp, const QString& value) { dp.w = value.toFloat(); dp.orientation.setScalar(value.toFloat()); }},
                                                                                   {"p_gpsLat", [](dataPoint& dp, const QString& value) { dp.p_gpsLat = value.toFloat() / pow(10, 7); }},
                                                                                   {"p_gpsLong", [](dataPoint& dp, const QString& value) { dp.p_gpsLong = value.toFloat() / pow(10, 7); }},
                                                                                   {"p_targetGpsLat", [](dataPoint& dp, const QString& value) { dp.p_targetGpsLat = value.toFloat(); }},
                                                                                   {"p_targetGpsLong", [](dataPoint& dp, const QString& value) { dp.p_targetGpsLong = value.toFloat(); }},
                                                                                   {"p_desiredServoPos", [](dataPoint& dp, const QString& value) { dp.desiredPayloadServo1Position = value.toInt(); }},
                                                                                   {"p_actualServoPos", [](dataPoint& dp, const QString& value) { dp.payloadServo1Position = value.toInt(); }},
                                                                                   {"gpsLock", [](dataPoint& dp, const QString& value) { dp.gpsLock = value.toLower() == "true"; }},
                                                                                   {"satellites", [](dataPoint& dp, const QString& value) { dp.numSatellites = value.toInt(); }},
                                                                                   {"epochTime", [](dataPoint& dp, const QString& value) { dp.epochTime = value.toInt(); }},
                                                                                   {"pressure", [](dataPoint& dp, const QString& value) { dp.pressure = value.toFloat(); }},
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
    void p_gpsPointUpdated(float, float);
    void p_targetGpsPointUpdated(float, float);
    void gpsLockUpdated();
    void numSatellitesUpdated();

public slots:
    void update();
    void onTextMessageReceived(QString message);

private:
    Ui::MainWindow *ui;



};
#endif // MAINWINDOW_H
