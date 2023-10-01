#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "qstyle.h"
#include <QTimer>
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setBackgroundRole(QPalette::Window);
    setPalette(QApplication::style()->standardPalette());
//    setupCentralWidget();
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(TIMER_TICK_MS);

    connect(&m_webSocket, SIGNAL(connected()), this, SLOT(onConnected()));
    connect(&m_webSocket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));

    m_webSocket.open(QUrl(QString("ws://127.0.0.1:3005")));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateData(dataPoint p)
{
#if RUN_SPEED_TESTS
    emit speedTick(1);
    return;
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
        emit altUpdated(p.altitude);
    }
    if(p.state != m_currentData.state)
    {
        if(p.state == 2)
        {
            m_groundLaunchTime = QDateTime::currentDateTimeUtc();
            m_rocketLaunchTime = p.rocketTime;
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
        m_currentData.rocketTime = p.rocketTime;
        m_currentData.accData.append(graphPoint {p.acceleration, p.rocketTime});
        while(m_currentData.accData.first().time < p.rocketTime - 5000)
            m_currentData.accData.removeFirst();
        m_currentData.velData.append(graphPoint {p.velocity, p.rocketTime});
        while(m_currentData.velData.first().time < p.rocketTime - 5000)
            m_currentData.velData.removeFirst();
        m_currentData.altData.append(graphPoint {p.altitude, p.rocketTime});
        while(m_currentData.altData.first().time < p.rocketTime - 5000)
            m_currentData.altData.removeFirst();
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
        emit orientationUpdated();
    }
    if(p.gyroX != m_currentData.gyroX || p.gyroY != m_currentData.gyroY || p.gyroZ != m_currentData.gyroZ)
    {
        m_currentData.gyroX = gyroX;
        m_currentData.gyroY = gyroY;
        m_currentData.gyroZ = gyroZ;
        emit gyroUpdated();
    }
    emit tick(); // for anything that should update at max speed; example would be a flashing light that can track its own alternating pattern or internal clock
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

void MainWindow::onConnected()
{
    connect(&m_webSocket, SIGNAL(textMessageReceived(QString)), this, SLOT(onTextMessageReceived(QString)));

}

void MainWindow::onDisconnected() {}

void MainWindow::onTextMessageReceived(QString message)
{
    QStringList messageSplit = message.split(",");
    for(QString e : messageSplit.toList())
    {
        e.remove("\"");
        QStringList elementSplit = e.split(":");
        if(elementSplit.at(0) == QString("Altitude"))
        {
            QString altData = elementSplit.at(1);
            altData.remove("}");
            m_dataBuffer.altitude = altData.toFloat();
        } else if(elementSplit.at(0) == QString("Velocity"))
        {
            m_dataBuffer.velocity = elementSplit.at(1).toFloat();
        } else if(elementSplit.at(0) == QString("AccelZ"))
        {
            m_dataBuffer.acceleration = elementSplit.at(1).toDouble();
        } else if(elementSplit.at(0) == QString("Timestamp"))
        {
            m_dataBuffer.rocketTime = elementSplit.at(1).toFloat();
        }
        else if(elementSplit.at(0) == QString("State"))
        {
            m_dataBuffer.state = elementSplit.at(1).toInt();
        }
    }

}
