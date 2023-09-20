#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qdatetime.h"
#include "qwebsocket.h"
#include <QMainWindow>
#include <QWebSocketServer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

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
    };






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

signals:
    void accUpdated(int);
    void velUpdated(int);
    void altUpdated(int);
    void stateUpdated(int);
    void rocketTimeUpdated(float);
    void groundTimeUpdated();
    void currentAirbrakesUpdated(float);
    void desiredAirbrakesUpdated(float);
    void tick();


public slots:
    void update();
    void onConnected();
    void onDisconnected();
    void onTextMessageReceived(QString message);

private:
    Ui::MainWindow *ui;



};
#endif // MAINWINDOW_H
