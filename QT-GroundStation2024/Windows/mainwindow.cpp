#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "qstyle.h"
#include <QTimer>
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    m_currentData.accData = graphPointCirularBufferCreate(CIRCULAR_BUFFER_LEN);
    m_currentData.velData = graphPointCirularBufferCreate(CIRCULAR_BUFFER_LEN);
    m_currentData.altData = graphPointCirularBufferCreate(CIRCULAR_BUFFER_LEN);


    ui->setupUi(this);
    setBackgroundRole(QPalette::Window);
    setPalette(QApplication::style()->standardPalette());
//    setupCentralWidget();
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(TIMER_TICK_MS);

    m_websocket = new HPRCWebSocket("hprc-test.entflammen.com", 8000);
    connect(m_websocket, SIGNAL(onTextMessageReceived(QString)), this, SLOT(onTextMessageReceived(QString)));
    m_websocket->connectToServer();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateData(dataPoint p)
{
#if RUN_SPEED_TESTS
    emit speedTick(1);
//    return;
#endif

    if(p.acceleration != m_currentData.acceleration)
    {
        m_currentData.acceleration = p.acceleration;

        emit accUpdated(p.acceleration);

    }
    if(p.velocity != m_currentData.velocity)
    {
        m_currentData.velocity = p.velocity;
        emit velUpdated(p.velocity);
    }
    if(p.altitude != m_currentData.altitude)
    {
        m_currentData.altitude = p.altitude;
        emit altUpdated(p.altitude, p.rocketTime);
    }
    if(p.state != m_currentData.state)
    {


        if(p.state == 2) // 2: launch 3: coast 4: drogue 6: main 8: landing
        {
            m_groundLaunchTime = QDateTime::currentDateTimeUtc();
            m_rocketLaunchTime = p.rocketTime;
            m_currentData.timelineActivated[4] = true;
        } else if(p.state == 3) {
            m_currentData.timelineActivated[3] = true;
            double t = m_currentData.rocketTimeSinceLaunch / 1000.0;

            int wholeNumSecs = t;
            int hours = wholeNumSecs / 60 / 60;
            int minutes = (wholeNumSecs - hours * 60 * 60) / 60;
            double seconds = (t - hours * 60.0 * 60.0 - minutes * 60.0);

            int rseconds = seconds * 100;
            seconds = rseconds/100;

            using namespace std;

            string hourString(to_string(hours));
            string minuteString(to_string(minutes));
            string secondString(to_string(seconds));

            if(hourString.length() == 1)
                hourString.insert(0, "0");
            if(minuteString.length() == 1)
                minuteString.insert(0, "0");
            if(secondString.at(1) == *".")
                secondString.insert(0, "0");
            secondString = secondString.substr(0, 2);
            QString timeString = QString::fromStdString(hourString + ":" + minuteString + ":" + secondString);
            m_currentData.timelineTimes[3] = timeString;
        } else if(p.state == 4) {
            m_currentData.timelineActivated[2] = true;
            double t = m_currentData.rocketTimeSinceLaunch / 1000.0;

            int wholeNumSecs = t;
            int hours = wholeNumSecs / 60 / 60;
            int minutes = (wholeNumSecs - hours * 60 * 60) / 60;
            double seconds = (t - hours * 60.0 * 60.0 - minutes * 60.0);

            int rseconds = seconds * 100;
            seconds = rseconds/100;

            using namespace std;

            string hourString(to_string(hours));
            string minuteString(to_string(minutes));
            string secondString(to_string(seconds));

            if(hourString.length() == 1)
                hourString.insert(0, "0");
            if(minuteString.length() == 1)
                minuteString.insert(0, "0");
            if(secondString.at(1) == *".")
                secondString.insert(0, "0");
            secondString = secondString.substr(0, 2);
            QString timeString = QString::fromStdString(hourString + ":" + minuteString + ":" + secondString);
            m_currentData.timelineTimes[2] = timeString;//            m_currentData.timelineTimes[2] = m_currentData.rocketTimeSinceLaunch;
        } else if(p.state == 6) {
            m_currentData.timelineActivated[1] = true;
            double t = m_currentData.rocketTimeSinceLaunch / 1000.0;

            int wholeNumSecs = t;
            int hours = wholeNumSecs / 60 / 60;
            int minutes = (wholeNumSecs - hours * 60 * 60) / 60;
            double seconds = (t - hours * 60.0 * 60.0 - minutes * 60.0);

            int rseconds = seconds * 100;
            seconds = rseconds/100;

            using namespace std;

            string hourString(to_string(hours));
            string minuteString(to_string(minutes));
            string secondString(to_string(seconds));

            if(hourString.length() == 1)
                hourString.insert(0, "0");
            if(minuteString.length() == 1)
                minuteString.insert(0, "0");
            if(secondString.at(1) == *".")
                secondString.insert(0, "0");
            secondString = secondString.substr(0, 2);
            QString timeString = QString::fromStdString(hourString + ":" + minuteString + ":" + secondString);
            m_currentData.timelineTimes[1] = timeString;//            m_currentData.timelineTimes[1] = m_currentData.rocketTimeSinceLaunch;
        } else if(p.state == 8) {
            m_currentData.timelineActivated[0] = true;
            double t = m_currentData.rocketTimeSinceLaunch / 1000.0;

            int wholeNumSecs = t;
            int hours = wholeNumSecs / 60 / 60;
            int minutes = (wholeNumSecs - hours * 60 * 60) / 60;
            double seconds = (t - hours * 60.0 * 60.0 - minutes * 60.0);

            int rseconds = seconds * 100;
            seconds = rseconds/100;

            using namespace std;

            string hourString(to_string(hours));
            string minuteString(to_string(minutes));
            string secondString(to_string(seconds));

            if(hourString.length() == 1)
                hourString.insert(0, "0");
            if(minuteString.length() == 1)
                minuteString.insert(0, "0");
            if(secondString.at(1) == *".")
                secondString.insert(0, "0");
            secondString = secondString.substr(0, 2);
            QString timeString = QString::fromStdString(hourString + ":" + minuteString + ":" + secondString);
            m_currentData.timelineTimes[0] = timeString;//            m_currentData.timelineTimes[0] = m_currentData.rocketTimeSinceLaunch;
        } else if(p.state == 1) { // reset when data returns to initial state
            m_currentData.timelineActivated[0] = false;
            m_currentData.timelineActivated[1] = false;
            m_currentData.timelineActivated[2] = false;
            m_currentData.timelineActivated[3] = false;
            m_currentData.timelineActivated[4] = false;
            m_currentData.timelineTimes[0] = "00:00:00";
            m_currentData.timelineTimes[1] = "00:00:00";
            m_currentData.timelineTimes[2] = "00:00:00";
            m_currentData.timelineTimes[3] = "00:00:00";
            m_currentData.timelineTimes[4] = "00:00:00";


        }
        m_currentData.state = p.state;
        emit stateUpdated(p.state);
    }
    if(p.rocketTime != m_currentData.rocketTime)
    {
        if(p.state > 1)
        {
            m_currentData.rocketTimeSinceLaunch = p.rocketTime - m_rocketLaunchTime;
        }

        if(p.state >= RocketState::DROGUE_DESCENT && p.state <= RocketState::MAIN_DESCENT) { //If the rocket is descending then the payload window needs data to stay in memory for longer
            dataDeletionTime = 60000*15; //15 minutes
        } else {
            dataDeletionTime = 5000;
        }

        m_currentData.rocketTime = p.rocketTime;
        graphPointCirularBufferAdd(m_currentData.accData, GraphPoint {
                                                              .value = p.acceleration,
                                                              .time = p.rocketTime
                                                          });
        graphPointCirularBufferAdd(m_currentData.velData, GraphPoint {
                                                              .value = p.velocity,
                                                              .time = p.rocketTime
                                                          });
        graphPointCirularBufferAdd(m_currentData.altData, GraphPoint {
                                                              .value = p.altitude,
                                                              .time = p.rocketTime
                                                          });


        m_currentData.accData1.append(graphPoint {p.acceleration, p.rocketTime});
        while(m_currentData.accData1.first().time < p.rocketTime - dataDeletionTime)
            m_currentData.accData1.removeFirst();
        m_currentData.velData1.append(graphPoint {p.velocity, p.rocketTime});
        while(m_currentData.velData1.first().time < p.rocketTime - dataDeletionTime)
            m_currentData.velData1.removeFirst();
        m_currentData.altData1.append(graphPoint {p.altitude, p.rocketTime});
        while(m_currentData.altData1.first().time < p.rocketTime - dataDeletionTime)
            m_currentData.altData1.removeFirst();

        emit rocketTimeUpdated(p.rocketTime);
    }
    if(p.groundTime != m_currentData.groundTime)
    {
        m_currentData.groundTime = p.groundTime;
        emit groundTimeUpdated();
    }
    if(p.orientation != m_currentData.orientation)
    {
        m_currentData.orientation = p.orientation;

        m_currentData.i = p.i;
        m_currentData.j = p.j;
        m_currentData.k = p.k;
        m_currentData.w = p.w;

        emit orientationUpdated();
    }
    if(p.currentAirbrakes != m_currentData.currentAirbrakes) {
        m_currentData.currentAirbrakes = p.currentAirbrakes;
        emit currentAirbrakesUpdated(p.currentAirbrakes);
    }
    if(p.desiredAirbrakes != m_currentData.desiredAirbrakes) {
        m_currentData.desiredAirbrakes = p.desiredAirbrakes;
        emit desiredAirbrakesUpdated(p.desiredAirbrakes);
    }
    if(p.gyroX != m_currentData.gyroX || p.gyroY != m_currentData.gyroY || p.gyroZ != m_currentData.gyroZ)
    {
        m_currentData.gyroX = p.gyroX;
        m_currentData.gyroY = p.gyroY;
        m_currentData.gyroZ = p.gyroZ;
        emit gyroUpdated();
    }
    if(p.payloadServo1Position != m_currentData.payloadServo1Position) {
        m_currentData.payloadServo1Position = p.payloadServo1Position;
        emit payloadServo1PositionUpdated(p.payloadServo1Position);
    }
    if(p.payloadServo2Position != m_currentData.payloadServo2Position) {
        m_currentData.payloadServo2Position = p.payloadServo2Position;
    }
    if(p.desiredPayloadServo1Position != m_currentData.desiredPayloadServo1Position) {
        m_currentData.desiredPayloadServo1Position = p.desiredPayloadServo1Position;
        emit desiredPayloadServo1PositionUpdated(p.desiredPayloadServo1Position);
    }
    if(p.desiredPayloadServo2Position != m_currentData.desiredPayloadServo2Position) {
        m_currentData.desiredPayloadServo2Position = p.desiredPayloadServo2Position;
    }

    if(p.payloadBatteryVoltage != m_currentData.payloadBatteryVoltage) {
        m_currentData.payloadBatteryVoltage = p.payloadBatteryVoltage;
    }

    if(p.gpsLock != m_currentData.gpsLock)
    {

        m_currentData.gpsLock = p.gpsLock;
        emit gpsLockUpdated();
    }

    if(p.gpsLock)
    {
        if(p.p_gpsLat != m_currentData.p_gpsLat || p.p_gpsLong != m_currentData.p_gpsLong)  {
            m_currentData.p_gpsLat = p.p_gpsLat;
            m_currentData.p_gpsLong = p.p_gpsLong;
            emit p_gpsPointUpdated(p.p_gpsLat, p.p_gpsLong);
        }
    }

    if(p.numSatellites != m_currentData.numSatellites)
    {
        m_currentData.numSatellites = p.numSatellites;
        emit numSatellitesUpdated();
    }

    if(p.p_targetGpsLat != m_currentData.p_targetGpsLat || p.p_targetGpsLong != m_currentData.p_targetGpsLong)  {
        m_currentData.p_targetGpsLat = p.p_targetGpsLat;
        m_currentData.p_targetGpsLong = p.p_targetGpsLong;
        emit p_gpsPointUpdated(p.p_targetGpsLat, p.p_targetGpsLong);
    }

    if(p.epochTime != m_currentData.epochTime)
    {
        m_currentData.epochTime = p.epochTime;
    }
    \
    if(p.pressure != m_currentData.pressure)
    {
        m_currentData.pressure = p.pressure;
    }

    emit tick(); // for anything that should update at max speed; example would be a flashing light that can track its own alternating pattern or internal clock
    emit gpsLockUpdated();
}

void MainWindow::update()
{
    if(m_dataBuffer.state > 1)
    {
        m_dataBuffer.groundTime = m_groundLaunchTime.msecsTo(QDateTime::currentDateTimeUtc());
    }
    pts += 1;

    updateData(m_dataBuffer);
}


void MainWindow::onTextMessageReceived(QString message)
{
    QStringList messageSplit = message.split(",");

    for(QString e : messageSplit.toList())
    {

        e.replace("}", "");
        e.replace("{", "");

        e.replace("\n", "");
        e.replace("\t", "");
        e.replace("\r", "");
        e.remove(QRegularExpression("[\"']"));
        e.remove("\"");
        QStringList elementSplit = e.split(":");

        // TODO: Make this switch-case
        if(elementSplit.at(0).toLower() == QString("altitude").toLower())
        {
            QString altData = elementSplit.at(1);
            altData.remove("}");
            m_dataBuffer.altitude = altData.toFloat();
        } else if(elementSplit.at(0).toLower() == QString("Velocity").toLower())
        {
            m_dataBuffer.velocity = elementSplit.at(1).toFloat();
        } else if(elementSplit.at(0).toLower() == QString("AccelZ").toLower())
        {
            m_dataBuffer.acceleration = elementSplit.at(1).toDouble();
        } else if(elementSplit.at(0).toLower() == QString("Timestamp").toLower())
        {
            m_dataBuffer.rocketTime = elementSplit.at(1).toFloat();
        }
        else if(elementSplit.at(0).toLower() == QString("State").toLower())
        {
            m_dataBuffer.state = elementSplit.at(1).toInt();
        }
        else if(elementSplit.at(0).toLower() == QString("AirbrakesDeploy").toLower())
        {
            m_dataBuffer.currentAirbrakes = elementSplit.at(1).toFloat()/200;
            m_dataBuffer.desiredAirbrakes = m_dataBuffer.currentAirbrakes * 2;
        }
        else if(elementSplit.at(0).toLower() == QString("i"))
        {
            m_dataBuffer.orientation.setX(elementSplit.at(1).toFloat());
            m_dataBuffer.i = elementSplit.at(1).toFloat();
        }
        else if(elementSplit.at(0).toLower() == QString("k"))
        {
            m_dataBuffer.orientation.setY(elementSplit.at(1).toFloat());
            m_dataBuffer.k = elementSplit.at(1).toFloat();
        }
        else if(elementSplit.at(0).toLower() == QString("j"))
        {
            m_dataBuffer.orientation.setZ(elementSplit.at(1).toFloat());
            m_dataBuffer.j = elementSplit.at(1).toFloat();
        }
        else if(elementSplit.at(0).toLower() == QString("w"))
        {
            m_dataBuffer.orientation.setScalar(elementSplit.at(1).toFloat());
            m_dataBuffer.w = elementSplit.at(1).toFloat();
        }
        else if(elementSplit.at(0).toLower() == QString("p_gpsLat").toLower())
        {
            m_dataBuffer.p_gpsLat = elementSplit.at(1).toFloat() / pow(10, 7);
        }
        else if(elementSplit.at(0).toLower() == QString("p_gpsLong").toLower())
        {
            m_dataBuffer.p_gpsLong = elementSplit.at(1).toFloat() / pow(10, 7);
        }
        else if(elementSplit.at(0).toLower() == QString("p_targetGPSLat").toLower())
        {
            m_dataBuffer.p_targetGpsLat = elementSplit.at(1).toFloat();
        }
        else if(elementSplit.at(0).toLower() == QString("p_targetGPSLong").toLower())
        {
            m_dataBuffer.p_targetGpsLong = elementSplit.at(1).toFloat();
        }
        else if(elementSplit.at(0).toLower() == QString("p_desiredServoPos").toLower())
        {
            m_dataBuffer.desiredPayloadServo1Position = elementSplit.at(1).toInt();
        }
        else if(elementSplit.at(0).toLower() == QString("p_actualServoPos").toLower())
        {
            m_dataBuffer.payloadServo1Position = elementSplit.at(1).toInt();
        }
        else if(elementSplit.at(0).toLower() == QString("gpsLock").toLower())
        {
            m_dataBuffer.gpsLock = elementSplit.at(1).toLower() == QString("true");
        }
        else if(elementSplit.at(0).toLower() == QString("satellites").toLower())
        {
            m_dataBuffer.numSatellites = elementSplit.at(1).toInt();
        }
        else if(elementSplit.at(0).toLower() == QString("epochTime").toLower())
        {
            m_dataBuffer.epochTime = elementSplit.at(1).toInt();
        }
        else if(elementSplit.at(0).toLower() == QString("pressure").toLower())
        {
            m_dataBuffer.pressure = elementSplit.at(1).toFloat();
        }
    }

}
