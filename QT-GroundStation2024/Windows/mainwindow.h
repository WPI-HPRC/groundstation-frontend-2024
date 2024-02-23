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

    QMap<QString, std::function<void(dataPoint&, const QJsonValue&)>> elementMap = {
                                                                                      {"epochTime", [](dataPoint& dp, const QJsonValue& value) { dp.epochTime = value.toInt();}},
                                                                                      {"timestamp", [](dataPoint& dp, const QJsonValue& value) { dp.rocketTime = value.toDouble(); }},
                                                                                      {"gpsLock", [](dataPoint& dp, const QJsonValue& value) { dp.gpsLock = value.toBool(); }},
                                                                                      {"state", [](dataPoint& dp, const QJsonValue& value) { dp.state = value.toInt(); }},

                                                                                      {"satellites", [](dataPoint& dp, const QJsonValue& value) { dp.numSatellites = value.toInt(); }},
                                                                                      {"gpsLat", [](dataPoint& dp, const QJsonValue& value) { dp.gpsLat = value.toDouble(); }},
                                                                                      {"gpsLong", [](dataPoint& dp, const QJsonValue& value) { dp.gpsLong = value.toDouble(); }},
                                                                                      {"gpsAltMSL", [](dataPoint& dp, const QJsonValue& value) { dp.gpsAltMSL = value.toDouble(); }},
                                                                                      {"gpsAltAGL", [](dataPoint& dp, const QJsonValue& value) { dp.gpsAltAGL = value.toDouble(); }},
                                                                                      {"pressure", [](dataPoint& dp, const QJsonValue& value) { dp.pressure = value.toDouble(); }},
                                                                                      {"altitude", [](dataPoint& dp, const QJsonValue& value) { dp.altitude = value.toDouble(); }},
                                                                                      {"accelX", [](dataPoint& dp, const QJsonValue& value) { dp.accelX = value.toDouble(); }},
                                                                                      {"accelY", [](dataPoint& dp, const QJsonValue& value) { dp.accelY = value.toDouble(); }},
                                                                                      {"accelZ", [](dataPoint& dp, const QJsonValue& value) { dp.accelZ = value.toDouble(); }},
                                                                                      {"gyroX", [](dataPoint& dp, const QJsonValue& value) { dp.gyroX = value.toDouble(); }},
                                                                                      {"gyroY", [](dataPoint& dp, const QJsonValue& value) { dp.gyroY = value.toDouble(); }},
                                                                                      {"gyroZ", [](dataPoint& dp, const QJsonValue& value) { dp.gyroZ = value.toDouble(); }},
                                                                                      {"magX", [](dataPoint& dp, const QJsonValue& value) { dp.magX = value.toDouble(); }},
                                                                                      {"magY", [](dataPoint& dp, const QJsonValue& value) { dp.magY = value.toDouble(); }},
                                                                                      {"magZ", [](dataPoint& dp, const QJsonValue& value) { dp.magZ = value.toDouble(); }},
                                                                                      {"i", [](dataPoint& dp, const QJsonValue& value) { dp.i = value.toDouble(); dp.orientation.setX(value.toDouble()); }},
                                                                                      {"k", [](dataPoint& dp, const QJsonValue& value) { dp.j = value.toDouble(); dp.orientation.setY(value.toDouble()); }},
                                                                                      {"j", [](dataPoint& dp, const QJsonValue& value) { dp.k = value.toDouble(); dp.orientation.setZ(value.toDouble()); }},
                                                                                      {"w", [](dataPoint& dp, const QJsonValue& value) { dp.w = value.toDouble(); dp.orientation.setScalar(value.toDouble()); }},
                                                                                      {"posX", [](dataPoint& dp, const QJsonValue& value) { dp.posX = value.toDouble(); }},
                                                                                      {"posY", [](dataPoint& dp, const QJsonValue& value) { dp.posY = value.toDouble(); }},
                                                                                      {"posZ", [](dataPoint& dp, const QJsonValue& value) { dp.posZ = value.toDouble(); }},
                                                                                      {"velX", [](dataPoint& dp, const QJsonValue& value) { dp.velX = value.toDouble(); }},
                                                                                      {"velY", [](dataPoint& dp, const QJsonValue& value) { dp.velY = value.toDouble(); }},
                                                                                      {"velZ", [](dataPoint& dp, const QJsonValue& value) { dp.velZ = value.toDouble(); }},

                                                                                      {"p_groundTime", [](dataPoint& dp, const QJsonValue& value) { dp.p_groundTime = value.toInt(); }},
                                                                                      {"p_epochTime", [](dataPoint& dp, const QJsonValue& value) { dp.p_epochTime = value.toInt(); }},
                                                                                      {"p_state", [](dataPoint& dp, const QJsonValue& value) { dp.p_state = value.toInt(); }},
                                                                                      {"p_gpsLock", [](dataPoint& dp, const QJsonValue& value) { dp.p_gpsLock = value.toBool(); }},
                                                                                      {"p_satellites", [](dataPoint& dp, const QJsonValue& value) { dp.p_numSatellites = value.toInt(); }},
                                                                                      {"p_gpsLat", [](dataPoint& dp, const QJsonValue& value) { dp.p_gpsLat = value.toDouble(); }},
                                                                                      {"p_gpsLong", [](dataPoint& dp, const QJsonValue& value) { dp.p_gpsLong = value.toDouble(); }},
                                                                                      {"p_gpsAltMSL", [](dataPoint& dp, const QJsonValue& value) { dp.p_gpsAltMSL = value.toDouble(); }},
                                                                                      {"p_gpsAltAGL", [](dataPoint& dp, const QJsonValue& value) { dp.p_gpsAltAGL = value.toDouble(); }},
                                                                                      {"p_pressure", [](dataPoint& dp, const QJsonValue& value) { dp.p_pressure = value.toDouble(); }},
                                                                                      {"p_altitude", [](dataPoint& dp, const QJsonValue& value) { dp.p_altitude = value.toDouble(); }},
                                                                                      {"p_accelX", [](dataPoint& dp, const QJsonValue& value) { dp.p_accelX = value.toDouble(); }},
                                                                                      {"p_accelY", [](dataPoint& dp, const QJsonValue& value) { dp.p_accelY = value.toDouble(); }},
                                                                                      {"p_accelZ", [](dataPoint& dp, const QJsonValue& value) { dp.p_accelZ = value.toDouble(); }},
                                                                                      {"p_gyroX", [](dataPoint& dp, const QJsonValue& value) { dp.p_gyroX = value.toDouble(); }},
                                                                                      {"p_gyroY", [](dataPoint& dp, const QJsonValue& value) { dp.p_gyroY = value.toDouble(); }},
                                                                                      {"p_gyroZ", [](dataPoint& dp, const QJsonValue& value) { dp.p_gyroZ = value.toDouble(); }},

                                                                                      {"p_magX", [](dataPoint& dp, const QJsonValue& value) { dp.magX = value.toDouble(); }},
                                                                                      {"p_magY", [](dataPoint& dp, const QJsonValue& value) { dp.magY = value.toDouble(); }},
                                                                                      {"p_magZ", [](dataPoint& dp, const QJsonValue& value) { dp.magZ = value.toDouble(); }},
                                                                                      {"p_i", [](dataPoint& dp, const QJsonValue& value) { dp.p_i = value.toDouble(); dp.p_orientation.setX(value.toDouble()); }},
                                                                                      {"p_k", [](dataPoint& dp, const QJsonValue& value) { dp.p_j = value.toDouble(); dp.p_orientation.setY(value.toDouble()); }},
                                                                                      {"p_j", [](dataPoint& dp, const QJsonValue& value) { dp.p_k = value.toDouble(); dp.p_orientation.setZ(value.toDouble()); }},
                                                                                      {"p_w", [](dataPoint& dp, const QJsonValue& value) { dp.p_w = value.toDouble(); dp.p_orientation.setScalar(value.toDouble()); }},
                                                                                      {"p_posX", [](dataPoint& dp, const QJsonValue& value) { dp.p_posX = value.toDouble(); }},
                                                                                      {"p_posY", [](dataPoint& dp, const QJsonValue& value) { dp.p_posY = value.toDouble(); }},
                                                                                      {"p_posZ", [](dataPoint& dp, const QJsonValue& value) { dp.p_posZ = value.toDouble(); }},
                                                                                      {"p_velX", [](dataPoint& dp, const QJsonValue& value) { dp.p_velX = value.toDouble(); }},
                                                                                      {"p_velY", [](dataPoint& dp, const QJsonValue& value) { dp.p_velY = value.toDouble(); }},
                                                                                      {"p_velZ", [](dataPoint& dp, const QJsonValue& value) { dp.p_velZ = value.toDouble(); }},

                                                                                      {"desiredAirbrakes", [](dataPoint& dp, const QJsonValue& value) { dp.desiredAirbrakes = value.toDouble(); }},
                                                                                      {"currentAirbrakes", [](dataPoint& dp, const QJsonValue& value) { dp.currentAirbrakes = value.toDouble(); }},
                                                                                      {"payloadBatteryVoltage", [](dataPoint& dp, const QJsonValue& value) { dp.payloadBatteryVoltage = value.toDouble(); }},
                                                                                      {"p_gpsLat", [](dataPoint& dp, const QJsonValue& value) { dp.p_gpsLat = value.toDouble() / pow(10, 7); }},
                                                                                      {"p_gpsLong", [](dataPoint& dp, const QJsonValue& value) { dp.p_gpsLong = value.toDouble() / pow(10, 7); }},
                                                                                      {"p_targetGpsLat", [](dataPoint& dp, const QJsonValue& value) { dp.p_targetGpsLat = value.toDouble(); }},
                                                                                      {"p_targetGpsLong", [](dataPoint& dp, const QJsonValue& value) { dp.p_targetGpsLong = value.toDouble(); }},
                                                                                      {"p_actualServoPos1", [](dataPoint& dp, const QJsonValue& value) { dp.p_actualServoPos1 = value.toDouble(); }},
                                                                                      {"p_desiredServoPos1", [](dataPoint& dp, const QJsonValue& value) { dp.p_desiredServoPos1 = value.toDouble(); }},
                                                                                      {"p_actualServoPos2", [](dataPoint& dp, const QJsonValue& value) { dp.p_actualServoPos2 = value.toDouble(); }},
                                                                                      {"p_desiredServoPos2", [](dataPoint& dp, const QJsonValue& value) { dp.p_desiredServoPos2 = value.toDouble(); }},
                                                                                      {"p_actualServoPos3", [](dataPoint& dp, const QJsonValue& value) { dp.p_actualServoPos3 = value.toDouble(); }},
                                                                                      {"p_desiredServoPos3", [](dataPoint& dp, const QJsonValue& value) { dp.p_desiredServoPos3 = value.toDouble(); }},
                                                                                      {"p_actualServoPos4", [](dataPoint& dp, const QJsonValue& value) { dp.p_actualServoPos4 = value.toDouble(); }},
                                                                                      {"p_desiredServoPos4", [](dataPoint& dp, const QJsonValue& value) { dp.p_desiredServoPos4 = value.toDouble(); }},
                                                                                      {"p_trajA", [](dataPoint& dp, const QJsonValue& value) { dp.p_trajA = value.toDouble(); }},
                                                                                      {"p_trajB", [](dataPoint& dp, const QJsonValue& value) { dp.p_trajB = value.toDouble(); }},
                                                                                      {"p_trajC", [](dataPoint& dp, const QJsonValue& value) { dp.p_trajC = value.toDouble(); }},
                                                                                      {"p_trajD", [](dataPoint& dp, const QJsonValue& value) { dp.p_trajD = value.toDouble(); }},
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
