#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "qstyle.h"
#include <QTimer>
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::MainWindow) {

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
    connect(m_websocket, SIGNAL(socketStateChanged(QAbstractSocket::SocketState)), this,
            SLOT(socketStateChanged(QAbstractSocket::SocketState)));
    m_websocket->connectToServer();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::socketStateChanged(QAbstractSocket::SocketState state) {
    emit webSocketStateChanged(state);
}


void MainWindow::updateData(dataPoint p) {
#if RUN_SPEED_TESTS
    emit speedTick(1);
//    return;
#endif

//    m_currentData = p;

    if (p.accelZ != m_currentData.accelZ) {
        m_currentData.accelZ = p.accelZ;

        emit accUpdated(p.accelZ);

    }
    if (p.velZ != m_currentData.velZ) {
        m_currentData.velZ = p.velZ;
        emit velUpdated(p.velZ);
    }
    if (p.altitude != m_currentData.altitude) {
        m_currentData.altitude = p.altitude;
        emit altUpdated(p.altitude, p.rocketTime);
    }
    if (p.state != m_currentData.state) {


        if (p.state == 2) // 2: launch 3: coast 4: drogue 6: main 8: landing
        {
            m_groundLaunchTime = QDateTime::currentDateTimeUtc();
            m_rocketLaunchTime = p.rocketTime;
            m_currentData.timelineActivated[4] = true;
        } else if (p.state == 3) {
            m_currentData.timelineActivated[3] = true;
            double t = m_currentData.rocketTimeSinceLaunch / 1000.0;

            int wholeNumSecs = t;
            int hours = wholeNumSecs / 60 / 60;
            int minutes = (wholeNumSecs - hours * 60 * 60) / 60;
            double seconds = (t - hours * 60.0 * 60.0 - minutes * 60.0);

            int rseconds = seconds * 100;
            seconds = rseconds / 100;

            std::string hourString(std::to_string(hours));
            std::string minuteString(std::to_string(minutes));
            std::string secondString(std::to_string(seconds));

            if (hourString.length() == 1)
                hourString.insert(0, "0");
            if (minuteString.length() == 1)
                minuteString.insert(0, "0");
            if (secondString.at(1) == *".")
                secondString.insert(0, "0");
            secondString = secondString.substr(0, 2);
            QString timeString = QString::fromStdString(hourString + ":" + minuteString + ":" + secondString);
            m_currentData.timelineTimes[3] = timeString;
        } else if (p.state == 4) {
            m_currentData.timelineActivated[2] = true;
            double t = m_currentData.rocketTimeSinceLaunch / 1000.0;

            int wholeNumSecs = t;
            int hours = wholeNumSecs / 60 / 60;
            int minutes = (wholeNumSecs - hours * 60 * 60) / 60;
            double seconds = (t - hours * 60.0 * 60.0 - minutes * 60.0);

            int rseconds = seconds * 100;
            seconds = rseconds / 100;

            std::string hourString(std::to_string(hours));
            std::string minuteString(std::to_string(minutes));
            std::string secondString(std::to_string(seconds));

            if (hourString.length() == 1)
                hourString.insert(0, "0");
            if (minuteString.length() == 1)
                minuteString.insert(0, "0");
            if (secondString.at(1) == *".")
                secondString.insert(0, "0");
            secondString = secondString.substr(0, 2);
            QString timeString = QString::fromStdString(hourString + ":" + minuteString + ":" + secondString);
            m_currentData.timelineTimes[2] = timeString;//            m_currentData.timelineTimes[2] = m_currentData.rocketTimeSinceLaunch;
        } else if (p.state == 6) {
            m_currentData.timelineActivated[1] = true;
            double t = m_currentData.rocketTimeSinceLaunch / 1000.0;

            int wholeNumSecs = t;
            int hours = wholeNumSecs / 60 / 60;
            int minutes = (wholeNumSecs - hours * 60 * 60) / 60;
            double seconds = (t - hours * 60.0 * 60.0 - minutes * 60.0);

            int rseconds = seconds * 100;
            seconds = rseconds / 100;

            std::string hourString(std::to_string(hours));
            std::string minuteString(std::to_string(minutes));
            std::string secondString(std::to_string(seconds));

            if (hourString.length() == 1)
                hourString.insert(0, "0");
            if (minuteString.length() == 1)
                minuteString.insert(0, "0");
            if (secondString.at(1) == *".")
                secondString.insert(0, "0");
            secondString = secondString.substr(0, 2);
            QString timeString = QString::fromStdString(hourString + ":" + minuteString + ":" + secondString);
            m_currentData.timelineTimes[1] = timeString;//            m_currentData.timelineTimes[1] = m_currentData.rocketTimeSinceLaunch;
        } else if (p.state == 8) {
            m_currentData.timelineActivated[0] = true;
            double t = m_currentData.rocketTimeSinceLaunch / 1000.0;

            int wholeNumSecs = t;
            int hours = wholeNumSecs / 60 / 60;
            int minutes = (wholeNumSecs - hours * 60 * 60) / 60;
            double seconds = (t - hours * 60.0 * 60.0 - minutes * 60.0);

            int rseconds = seconds * 100;
            seconds = rseconds / 100;

            std::string hourString(std::to_string(hours));
            std::string minuteString(std::to_string(minutes));
            std::string secondString(std::to_string(seconds));

            if (hourString.length() == 1)
                hourString.insert(0, "0");
            if (minuteString.length() == 1)
                minuteString.insert(0, "0");
            if (secondString.at(1) == *".")
                secondString.insert(0, "0");
            secondString = secondString.substr(0, 2);
            QString timeString = QString::fromStdString(hourString + ":" + minuteString + ":" + secondString);
            m_currentData.timelineTimes[0] = timeString;//            m_currentData.timelineTimes[0] = m_currentData.rocketTimeSinceLaunch;
        } else if (p.state == 1) { // reset when data returns to initial state
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
    if (p.rocketTime != m_currentData.rocketTime) {
        if (p.state > 1) {
            m_currentData.rocketTimeSinceLaunch = p.rocketTime - m_rocketLaunchTime;
        }

        if (p.state >= RocketState::DROGUE_DESCENT && p.state <=
                                                      RocketState::MAIN_DESCENT) { //If the rocket is descending then the payload window needs data to stay in memory for longer
            dataDeletionTime = 60000 * 15; //15 minutes
        } else {
            dataDeletionTime = 5000;
        }

        m_currentData.rocketTime = p.rocketTime;
        graphPointCirularBufferAdd(m_currentData.accData, GraphPoint{
                .value = p.accelZ,
                .time = p.rocketTime
        });
        graphPointCirularBufferAdd(m_currentData.velData, GraphPoint{
                .value = p.velZ,
                .time = p.rocketTime
        });
        graphPointCirularBufferAdd(m_currentData.altData, GraphPoint{
                .value = p.altitude,
                .time = p.rocketTime
        });


        m_currentData.accData1.append(graphPoint{p.accelZ, p.rocketTime});
        while (m_currentData.accData1.first().time < p.rocketTime - dataDeletionTime)
            m_currentData.accData1.removeFirst();
        m_currentData.velData1.append(graphPoint{p.velZ, p.rocketTime});
        while (m_currentData.velData1.first().time < p.rocketTime - dataDeletionTime)
            m_currentData.velData1.removeFirst();
        m_currentData.altData1.append(graphPoint{p.altitude, p.rocketTime});
        while (m_currentData.altData1.first().time < p.rocketTime - dataDeletionTime)
            m_currentData.altData1.removeFirst();

        emit rocketTimeUpdated(p.rocketTime);
    }
    if (p.groundTime != m_currentData.groundTime) {
        m_currentData.groundTime = p.groundTime;
        emit groundTimeUpdated();
    }
    if (p.orientation != m_currentData.orientation) {
        m_currentData.orientation = p.orientation;

        m_currentData.i = p.i;
        m_currentData.j = p.j;
        m_currentData.k = p.k;
        m_currentData.w = p.w;

        emit orientationUpdated();
    }
    if (p.currentAirbrakes != m_currentData.currentAirbrakes) {
        m_currentData.currentAirbrakes = p.currentAirbrakes;
        emit currentAirbrakesUpdated(p.currentAirbrakes);
    }
    if (p.desiredAirbrakes != m_currentData.desiredAirbrakes) {
        m_currentData.desiredAirbrakes = p.desiredAirbrakes;
        emit desiredAirbrakesUpdated(p.desiredAirbrakes);
    }
    if (p.gyroX != m_currentData.gyroX || p.gyroY != m_currentData.gyroY || p.gyroZ != m_currentData.gyroZ) {
        m_currentData.gyroX = p.gyroX;
        m_currentData.gyroY = p.gyroY;
        m_currentData.gyroZ = p.gyroZ;
        emit gyroUpdated();
    }
    if (p.p_actualServoPos1 != m_currentData.p_actualServoPos1) {
        m_currentData.p_actualServoPos1 = p.p_actualServoPos1;
        emit payloadServo1PositionUpdated(p.p_actualServoPos1);
    }
    if (p.p_actualServoPos2 != m_currentData.p_actualServoPos2) {
        m_currentData.p_actualServoPos2 = p.p_actualServoPos2;
    }
    if (p.p_desiredServoPos1 != m_currentData.p_desiredServoPos1) {
        m_currentData.p_desiredServoPos1 = p.p_desiredServoPos1;
        emit desiredPayloadServo1PositionUpdated(p.p_desiredServoPos1);
    }
    if (p.p_desiredServoPos2 != m_currentData.p_desiredServoPos2) {
        m_currentData.p_desiredServoPos2 = p.p_desiredServoPos2;
    }

    if (p.payloadBatteryVoltage != m_currentData.payloadBatteryVoltage) {
        m_currentData.payloadBatteryVoltage = p.payloadBatteryVoltage;
    }

    if (p.gpsLock != m_currentData.gpsLock) {

        m_currentData.gpsLock = p.gpsLock;
        emit gpsLockUpdated();
    }

    if (p.gpsLock) {
        if (p.p_gpsLat != m_currentData.p_gpsLat || p.p_gpsLong != m_currentData.p_gpsLong) {
            m_currentData.p_gpsLat = p.p_gpsLat;
            m_currentData.p_gpsLong = p.p_gpsLong;
            emit p_gpsPointUpdated(p.p_gpsLat, p.p_gpsLong);
        }
    }

    if (p.numSatellites != m_currentData.numSatellites) {
        m_currentData.numSatellites = p.numSatellites;
        emit numSatellitesUpdated();
    }

    if (p.p_targetGpsLat != m_currentData.p_targetGpsLat || p.p_targetGpsLong != m_currentData.p_targetGpsLong) {
        m_currentData.p_targetGpsLat = p.p_targetGpsLat;
        m_currentData.p_targetGpsLong = p.p_targetGpsLong;
        emit p_gpsPointUpdated(p.p_targetGpsLat, p.p_targetGpsLong);
    }

    if (p.epochTime != m_currentData.epochTime) {
        m_currentData.epochTime = p.epochTime;
    }

    if (p.pressure != m_currentData.pressure) {
        m_currentData.pressure = p.pressure;
    }


    m_currentData.groundTime = p.groundTime;
    m_currentData.epochTime = p.epochTime;
    m_currentData.rocketTime = p.rocketTime;
    m_currentData.rocketTimeSinceLaunch = p.rocketTimeSinceLaunch;
    m_currentData.state = p.state;

    m_currentData.gpsLock = p.gpsLock;
    m_currentData.numSatellites = p.numSatellites;
    m_currentData.gpsLat = p.gpsLat;
    m_currentData.gpsLong = p.gpsLong;
    m_currentData.gpsAltMSL = p.gpsAltMSL;
    m_currentData.gpsAltAGL = p.gpsAltAGL;

    m_currentData.pressure = p.pressure;
    m_currentData.altitude = p.altitude;

    m_currentData.accelX = p.accelX;
    m_currentData.accelY = p.accelY;
    m_currentData.accelZ = p.accelZ;
    m_currentData.gyroX = p.gyroX;
    m_currentData.gyroY = p.gyroY;
    m_currentData.gyroZ = p.gyroZ;
    m_currentData.magX = p.magX;
    m_currentData.magY = p.magY;
    m_currentData.magZ = p.magZ;
    m_currentData.i = p.i;
    m_currentData.j = p.j;
    m_currentData.k = p.k;
    m_currentData.w = p.w;
    m_currentData.orientation = p.orientation;
    m_currentData.posX = p.posX;
    m_currentData.posY = p.posY;
    m_currentData.posZ = p.posZ;
    m_currentData.velX = p.velX;
    m_currentData.velY = p.velY;
    m_currentData.velZ = p.velZ;

    m_currentData.desiredAirbrakes = p.desiredAirbrakes;
    m_currentData.currentAirbrakes = p.currentAirbrakes;

    m_currentData.p_groundTime = p.p_groundTime;
    m_currentData.p_epochTime = p.p_epochTime;
    m_currentData.p_rocketTime = p.p_rocketTime;
    m_currentData.p_rocketTimeSinceLaunch = p.p_rocketTimeSinceLaunch;
    m_currentData.p_state = p.p_state;

    m_currentData.p_gpsLock = p.p_gpsLock;
    m_currentData.p_numSatellites = p.p_numSatellites;
    m_currentData.p_gpsLat = p.p_gpsLat;
    m_currentData.p_gpsLong = p.p_gpsLong;
    m_currentData.p_gpsAltMSL = p.p_gpsAltMSL;
    m_currentData.p_gpsAltAGL = p.p_gpsAltAGL;

    m_currentData.p_pressure = p.p_pressure;
    m_currentData.p_altitude = p.p_altitude;

    m_currentData.p_accelX = p.p_accelX;
    m_currentData.p_accelY = p.p_accelY;
    m_currentData.p_accelZ = p.p_accelZ;
    m_currentData.p_gyroX = p.p_gyroX;
    m_currentData.p_gyroY = p.p_gyroY;
    m_currentData.p_gyroZ = p.p_gyroZ;
    m_currentData.p_magX = p.p_magX;
    m_currentData.p_magY = p.p_magY;
    m_currentData.p_magZ = p.p_magZ;
    m_currentData.p_i = p.p_i;
    m_currentData.p_j = p.p_j;
    m_currentData.p_k = p.p_k;
    m_currentData.p_w = p.p_w;
    m_currentData.p_orientation = p.p_orientation;
    m_currentData.p_posX = p.p_posX;
    m_currentData.p_posY = p.p_posY;
    m_currentData.p_posZ = p.p_posZ;
    m_currentData.p_velX = p.p_velX;
    m_currentData.p_velY = p.p_velY;
    m_currentData.p_velZ = p.p_velZ;

    m_currentData.p_cx = p.p_cx;
    m_currentData.p_cy = p.p_cy;
    m_currentData.p_targetGpsLat = p.p_targetGpsLat;
    m_currentData.p_targetGpsLong = p.p_targetGpsLong;

    m_currentData.p_actualServoPos1 = p.p_actualServoPos1;
    m_currentData.p_desiredServoPos1 = p.p_desiredServoPos1;
    m_currentData.p_actualServoPos2 = p.p_actualServoPos2;
    m_currentData.p_desiredServoPos2 = p.p_desiredServoPos2;
    m_currentData.p_actualServoPos3 = p.p_actualServoPos3;
    m_currentData.p_desiredServoPos3 = p.p_desiredServoPos3;
    m_currentData.p_actualServoPos4 = p.p_actualServoPos4;
    m_currentData.p_desiredServoPos4 = p.p_desiredServoPos4;
    m_currentData.payloadBatteryVoltage = p.payloadBatteryVoltage;

    m_currentData.p_trajA = p.p_trajA;
    m_currentData.p_trajB = p.p_trajB;
    m_currentData.p_trajC = p.p_trajC;
    m_currentData.p_trajD = p.p_trajD;

    emit tick(); // for anything that should update at max speed; example would be a flashing light that can track its own alternating pattern or internal clock
}

void MainWindow::update() {
    if (m_dataBuffer.state > 1) {
        m_dataBuffer.groundTime = m_groundLaunchTime.msecsTo(QDateTime::currentDateTimeUtc());
    }
    pts += 1;

    updateData(m_dataBuffer);
}


void MainWindow::onTextMessageReceived(QString message) {
    // Attempt to parse the string as JSON
    QJsonDocument jsonDocument = QJsonDocument::fromJson(message.toUtf8());

    if (jsonDocument.isObject()) {
        // Extract the JSON object
        QJsonObject jsonObject = jsonDocument.object();

        // Iterate over the keys in the JSON object
        for (auto it = jsonObject.begin(); it != jsonObject.end(); ++it) {
            QString elementName = it.key();
            QJsonValue elementValue = it.value();

            // Check if the element name is in the map
            if (elementMap.contains(elementName)) {
                // Use the conversion function to update the struct member
                elementMap[elementName](m_dataBuffer, elementValue);
            } else {
                qDebug() << "Key not recognized: " << elementName;
            }
            // Handle other cases if needed
        }
    }
    // Handle other cases if needed

}
