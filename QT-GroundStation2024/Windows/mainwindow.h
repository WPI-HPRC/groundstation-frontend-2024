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
        int groundTime = 0;
        int epochTime;
        float rocketTime = 0;
        float rocketTimeSinceLaunch = 0;
        int state = 0;

        bool gpsLock = false;
        int numSatellites = 0;
        float gpsLat = 0, gpsLong = 0;
        float gpsAltMSL = 0, gpsAltAGL = 0;

        float pressure = 0;
        float altitude = 0;

        float accelX = 0, accelY = 0, accelZ = 0;
        float gyroX = 0, gyroY = 0, gyroZ = 0;
        float magX = 0, magY = 0, magZ = 0;
        float i = 0, j = 0, k = 0, w = 0;
        QQuaternion orientation;
        float posX = 0, posY = 0, posZ = 0;
        float velX = 0, velY = 0, velZ = 0;

        float desiredAirbrakes = 0;
        float currentAirbrakes = 0;
        GraphPointCircularBuffer *accData;
        GraphPointCircularBuffer *velData;
        GraphPointCircularBuffer *altData;

        QList<graphPoint> accData1;
        QList<graphPoint> velData1;
        QList<graphPoint> altData1;

        bool timelineActivated[5] = {false, false, false, false, false};
        QString timelineTimes[5] = {"00:00:00", "00:00:00", "00:00:00", "00:00:00", "00:00:00"};

        int p_groundTime = 0;
        int p_epochTime;
        float p_rocketTime = 0;
        float p_rocketTimeSinceLaunch = 0;
        int p_state = 0;

        bool p_gpsLock;
        int p_numSatellites;
        float p_gpsLat, p_gpsLong;
        float p_gpsAltMSL, p_gpsAltAGL;

        float p_pressure;
        float p_altitude;

        float p_accelX, p_accelY, p_accelZ;
        float p_gyroX, p_gyroY, p_gyroZ;
        float p_magX, p_magY,p_magZ;
        float p_i, p_j, p_k, p_w;
        QQuaternion p_orientation;
        float p_posX, p_posY, p_posZ;
        float p_velX, p_velY, p_velZ;

        uint32_t p_cx;
        uint32_t p_cy;
        float p_targetGpsLat; //Target Point GPS Estimations
        float p_targetGpsLong;

        float p_actualServoPos1;
        float p_desiredServoPos1;
        float p_actualServoPos2;
        float p_desiredServoPos2;
        float p_actualServoPos3;
        float p_desiredServoPos3;
        float p_actualServoPos4;
        float p_desiredServoPos4;
        float payloadBatteryVoltage;

        float p_trajA;
        float p_trajB;
        float p_trajC;
        float p_trajD;

    };

    QMap<QString, std::function<void(dataPoint&, const QString&)>> elementMap = {
                                                                                   {"epochTime", [](dataPoint& dp, const QString& value) { dp.epochTime = value.toInt(); }},
                                                                                   {"timestamp", [](dataPoint& dp, const QString& value) { dp.rocketTime = value.toFloat(); }},
                                                                                   {"gpsLock", [](dataPoint& dp, const QString& value) { dp.gpsLock = value.toLower() == "true"; }},
                                                                                   {"satellites", [](dataPoint& dp, const QString& value) { dp.numSatellites = value.toInt(); }},
                                                                                   {"gpsLat", [](dataPoint& dp, const QString& value) { dp.gpsLat = value.toFloat(); }},
                                                                                   {"gpsLong", [](dataPoint& dp, const QString& value) { dp.gpsLong = value.toFloat(); }},
                                                                                   {"gpsAltMSL", [](dataPoint& dp, const QString& value) { dp.gpsAltMSL = value.toFloat(); }},
                                                                                   {"gpsAltAGL", [](dataPoint& dp, const QString& value) { dp.gpsAltAGL = value.toFloat(); }},
                                                                                   {"pressure", [](dataPoint& dp, const QString& value) { dp.pressure = value.toFloat(); }},
                                                                                   {"altitude", [](dataPoint& dp, const QString& value) { dp.altitude = value.toFloat(); }},
                                                                                   {"accelX", [](dataPoint& dp, const QString& value) { dp.accelX = value.toFloat(); }},
                                                                                   {"accelY", [](dataPoint& dp, const QString& value) { dp.accelY = value.toFloat(); }},
                                                                                   {"accelZ", [](dataPoint& dp, const QString& value) { dp.accelZ = value.toFloat(); }},
                                                                                   {"gyroX", [](dataPoint& dp, const QString& value) { dp.gyroX = value.toFloat(); }},
                                                                                   {"gyroY", [](dataPoint& dp, const QString& value) { dp.gyroY = value.toFloat(); }},
                                                                                   {"gyroZ", [](dataPoint& dp, const QString& value) { dp.gyroZ = value.toFloat(); }},
                                                                                   {"i", [](dataPoint& dp, const QString& value) { dp.i = value.toFloat(); dp.orientation.setX(value.toFloat()); }},
                                                                                   {"k", [](dataPoint& dp, const QString& value) { dp.j = value.toFloat(); dp.orientation.setY(value.toFloat()); }},
                                                                                   {"j", [](dataPoint& dp, const QString& value) { dp.k = value.toFloat(); dp.orientation.setZ(value.toFloat()); }},
                                                                                   {"w", [](dataPoint& dp, const QString& value) { dp.w = value.toFloat(); dp.orientation.setScalar(value.toFloat()); }},
                                                                                   {"posX", [](dataPoint& dp, const QString& value) { dp.posX = value.toFloat(); }},
                                                                                   {"posY", [](dataPoint& dp, const QString& value) { dp.posY = value.toFloat(); }},
                                                                                   {"posZ", [](dataPoint& dp, const QString& value) { dp.posZ = value.toFloat(); }},
                                                                                   {"velX", [](dataPoint& dp, const QString& value) { dp.velX = value.toFloat(); }},
                                                                                   {"velY", [](dataPoint& dp, const QString& value) { dp.velY = value.toFloat(); }},
                                                                                   {"velZ", [](dataPoint& dp, const QString& value) { dp.velZ = value.toFloat(); }},

                                                                                   {"p_groundTime", [](dataPoint& dp, const QString& value) { dp.p_groundTime = value.toInt(); }},
                                                                                   {"p_epochTime", [](dataPoint& dp, const QString& value) { dp.p_epochTime = value.toInt(); }},
                                                                                   {"p_state", [](dataPoint& dp, const QString& value) { dp.p_state = value.toInt(); }},
                                                                                   {"p_gpsLock", [](dataPoint& dp, const QString& value) { dp.p_gpsLock = value.toLower() == "true"; }},
                                                                                   {"p_satellites", [](dataPoint& dp, const QString& value) { dp.p_numSatellites = value.toInt(); }},
                                                                                   {"p_gpsLat", [](dataPoint& dp, const QString& value) { dp.p_gpsLat = value.toFloat(); }},
                                                                                   {"p_gpsLong", [](dataPoint& dp, const QString& value) { dp.p_gpsLong = value.toFloat(); }},
                                                                                   {"p_gpsAltMSL", [](dataPoint& dp, const QString& value) { dp.p_gpsAltMSL = value.toFloat(); }},
                                                                                   {"p_gpsAltAGL", [](dataPoint& dp, const QString& value) { dp.p_gpsAltAGL = value.toFloat(); }},
                                                                                   {"p_pressure", [](dataPoint& dp, const QString& value) { dp.p_pressure = value.toFloat(); }},
                                                                                   {"p_altitude", [](dataPoint& dp, const QString& value) { dp.p_altitude = value.toFloat(); }},
                                                                                   {"p_accelX", [](dataPoint& dp, const QString& value) { dp.p_accelX = value.toFloat(); }},
                                                                                   {"p_accelY", [](dataPoint& dp, const QString& value) { dp.p_accelY = value.toFloat(); }},
                                                                                   {"p_accelZ", [](dataPoint& dp, const QString& value) { dp.p_accelZ = value.toFloat(); }},
                                                                                   {"p_gyroX", [](dataPoint& dp, const QString& value) { dp.p_gyroX = value.toFloat(); }},
                                                                                   {"p_gyroY", [](dataPoint& dp, const QString& value) { dp.p_gyroY = value.toFloat(); }},
                                                                                   {"p_gyroZ", [](dataPoint& dp, const QString& value) { dp.p_gyroZ = value.toFloat(); }},
                                                                                   {"p_i", [](dataPoint& dp, const QString& value) { dp.p_i = value.toFloat(); dp.p_orientation.setX(value.toFloat()); }},
                                                                                   {"p_k", [](dataPoint& dp, const QString& value) { dp.p_j = value.toFloat(); dp.p_orientation.setY(value.toFloat()); }},
                                                                                   {"p_j", [](dataPoint& dp, const QString& value) { dp.p_k = value.toFloat(); dp.p_orientation.setZ(value.toFloat()); }},
                                                                                   {"p_w", [](dataPoint& dp, const QString& value) { dp.p_w = value.toFloat(); dp.p_orientation.setScalar(value.toFloat()); }},
                                                                                   {"p_posX", [](dataPoint& dp, const QString& value) { dp.p_posX = value.toFloat(); }},
                                                                                   {"p_posY", [](dataPoint& dp, const QString& value) { dp.p_posY = value.toFloat(); }},
                                                                                   {"p_posZ", [](dataPoint& dp, const QString& value) { dp.p_posZ = value.toFloat(); }},
                                                                                   {"p_velX", [](dataPoint& dp, const QString& value) { dp.p_velX = value.toFloat(); }},
                                                                                   {"p_velY", [](dataPoint& dp, const QString& value) { dp.p_velY = value.toFloat(); }},
                                                                                   {"p_velZ", [](dataPoint& dp, const QString& value) { dp.p_velZ = value.toFloat(); }},

                                                                                   {"desiredAirbrakes", [](dataPoint& dp, const QString& value) { dp.desiredAirbrakes = value.toFloat(); }},
                                                                                   {"currentAirbrakes", [](dataPoint& dp, const QString& value) { dp.currentAirbrakes = value.toFloat(); }},
                                                                                   {"payloadBatteryVoltage", [](dataPoint& dp, const QString& value) { dp.payloadBatteryVoltage = value.toFloat(); }},
                                                                                   {"p_gpsLat", [](dataPoint& dp, const QString& value) { dp.p_gpsLat = value.toFloat() / pow(10, 7); }},
                                                                                   {"p_gpsLong", [](dataPoint& dp, const QString& value) { dp.p_gpsLong = value.toFloat() / pow(10, 7); }},
                                                                                   {"p_targetGpsLat", [](dataPoint& dp, const QString& value) { dp.p_targetGpsLat = value.toFloat(); }},
                                                                                   {"p_targetGpsLong", [](dataPoint& dp, const QString& value) { dp.p_targetGpsLong = value.toFloat(); }},
                                                                                   {"p_actualServoPos1", [](dataPoint& dp, const QString& value) { dp.p_actualServoPos1 = value.toFloat(); }},
                                                                                   {"p_desiredServoPos1", [](dataPoint& dp, const QString& value) { dp.p_desiredServoPos1 = value.toFloat(); }},
                                                                                   {"p_actualServoPos2", [](dataPoint& dp, const QString& value) { dp.p_actualServoPos2 = value.toFloat(); }},
                                                                                   {"p_desiredServoPos2", [](dataPoint& dp, const QString& value) { dp.p_desiredServoPos2 = value.toFloat(); }},
                                                                                   {"p_actualServoPos3", [](dataPoint& dp, const QString& value) { dp.p_actualServoPos3 = value.toFloat(); }},
                                                                                   {"p_desiredServoPos3", [](dataPoint& dp, const QString& value) { dp.p_desiredServoPos3 = value.toFloat(); }},
                                                                                   {"p_actualServoPos4", [](dataPoint& dp, const QString& value) { dp.p_actualServoPos4 = value.toFloat(); }},
                                                                                   {"p_desiredServoPos4", [](dataPoint& dp, const QString& value) { dp.p_desiredServoPos4 = value.toFloat(); }},
                                                                                   {"p_trajA", [](dataPoint& dp, const QString& value) { dp.p_trajA = value.toFloat(); }},
                                                                                   {"p_trajB", [](dataPoint& dp, const QString& value) { dp.p_trajB = value.toFloat(); }},
                                                                                   {"p_trajC", [](dataPoint& dp, const QString& value) { dp.p_trajC = value.toFloat(); }},
                                                                                   {"p_trajD", [](dataPoint& dp, const QString& value) { dp.p_trajD = value.toFloat(); }},
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
