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
    timer->start(10);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateData(dataPoint p)
{
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
        m_currentData.state = p.state;
        emit stateUpdated(p.state);
    }
    if(p.rocketTime != m_currentData.rocketTime)
    {
        m_currentData.rocketTime = p.rocketTime;
        emit rocketTimeUpdated(p.rocketTime);
    }
    if(p.utcTime != m_currentData.utcTime)
    {
        m_currentData.utcTime = p.utcTime;
        emit utcTimeUpdated();
    }

}

void MainWindow::update()
{
    dataPoint newData(m_currentData);
    newData.utcTime += 1;
    updateData(newData);
}
