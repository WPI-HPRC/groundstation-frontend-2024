#include "hprcdisplaywidget.h"
#include "mainwindow.h"
#include "qapplication.h"
#include "qpainter.h"
#include "qstyle.h"
#include "qstyleoption.h"
#include <iostream>
#include "QPropertyAnimation"
#include <QMouseEvent>
#include <QQuaternion>

// 3D imports
#include <Qt3DCore/QEntity>
#include <Qt3DRender/QMesh>
#include <Qt3DRender/QCamera>
#include <Qt3DCore/QTransform>

#include <Qt3DExtras/QForwardRenderer>
#include <Qt3DExtras/QDiffuseSpecularMaterial>
#include <Qt3DExtras/Qt3DWindow>

hprcDisplayWidget::hprcDisplayWidget(QWidget *parent)
    : QWidget{parent}
{
    m_widgetType = HPRC_Hidden;
    m_filledPercent = 0;
    m_label = QString("TESTING");

#if RUN_SPEED_TESTS
    foreach (QWidget *w, qApp->topLevelWidgets())
        if (MainWindow* mainWin = qobject_cast<MainWindow*>(w))
        {
            connect(mainWin, SIGNAL(speedTick(int)), this, SLOT(doSpeedTick(int)));
        }
#endif
}

void hprcDisplayWidget::paintEvent(QPaintEvent *e)
{
    QPainter p(this);
    const QStyleOption opt(QStyleOption::Version, QStyleOption::SO_Default);
    QApplication::style()->drawPrimitive(QStyle::PE_CustomBase, &opt, &p, this);
}

hprcTimeline::hprcTimeline(QWidget *parent)
    : hprcDisplayWidget{parent}
{
    m_widgetType = HPRC_Timeline;
}

hprcGauge::hprcGauge(QWidget *parent)
    : hprcDisplayWidget{parent}
{
    m_widgetType = HPRC_Gauge;
}

void hprcDisplayWidget::updateFilled(float input)
{

    QPropertyAnimation *animation = new QPropertyAnimation(this, "filledPercent");
    animation->setDuration(400);
    animation->setEndValue((int) input);
    animation->start();
}

void hprcDisplayWidget::doSpeedTick(int input)
{
    repaint();
//    updateFilled(input);
}

int hprcDisplayWidget::getFilled()
{

    return m_filledPercent;
}
void hprcDisplayWidget::setFilled(int input)
{
    m_filledPercent = input;
    update();
}
int hprcDisplayWidget::fillChanged()
{

    return m_filledPercent;
}

hprcAltitudeGauge::hprcAltitudeGauge(QWidget *parent) :
    hprcGauge{parent}
{
    m_max = 12000.0;
    m_label = "ALTITUDE";
    foreach (QWidget *w, qApp->topLevelWidgets())
        if (MainWindow* mainWin = qobject_cast<MainWindow*>(w))
        {
            connect(mainWin, SIGNAL(altUpdated(float, float)), this, SLOT(updateFilled(float)));
        }
}

hprcVelocityGauge::hprcVelocityGauge(QWidget *parent) :
    hprcGauge{parent}
{
    m_max = 1000.0;
    m_label = "VELOCITY";
    foreach (QWidget *w, qApp->topLevelWidgets())
        if (MainWindow* mainWin = qobject_cast<MainWindow*>(w))
        {
            connect(mainWin, SIGNAL(velUpdated(int)), this, SLOT(updateFilled(int)));
        }
}

hprcAccelerationGauge::hprcAccelerationGauge(QWidget *parent) :
    hprcGauge{parent}
{
    m_label = "ACCELERATION";
    m_max = 400;
    foreach (QWidget *w, qApp->topLevelWidgets())
        if (MainWindow* mainWin = qobject_cast<MainWindow*>(w))
        {
            connect(mainWin, SIGNAL(accUpdated(int)), this, SLOT(updateFilled(int)));
        }
}

hprcAttitudeWidget::hprcAttitudeWidget(QWidget *parent):
    hprcDisplayWidget(parent)
{
//    setMouseTracking(true);
    m_widgetType = HPRC_Attitude;
    foreach (QWidget *w, qApp->topLevelWidgets())
        if (MainWindow* mainWin = qobject_cast<MainWindow*>(w))
        {
            connect(mainWin, SIGNAL(orientationUpdated()), this, SLOT(repaint()));
            connect(mainWin, SIGNAL(gyroUpdated()), this, SLOT(repaint()));
        }
}

void hprcAttitudeWidget::mouseMoveEvent(QMouseEvent *e)
{
    m_mousePos = e->pos();
}

hprcGraph::hprcGraph(QWidget *parent) :
    hprcDisplayWidget(parent)
{
    setMouseTracking(true);
    m_widgetType = HPRC_Graph;
    foreach (QWidget *w, qApp->topLevelWidgets())
        if (MainWindow* mainWin = qobject_cast<MainWindow*>(w))
        {
            connect(mainWin, SIGNAL(tick()), this, SLOT(repaint()));
        }
}

hprcPayloadGraph::hprcPayloadGraph(QWidget *parent) :
    hprcDisplayWidget(parent)
{
    setMouseTracking(true);
    m_widgetType = HPRC_PAYLOAD_GRAPH;
    foreach (QWidget *w, qApp->topLevelWidgets())
        if (MainWindow* mainWin = qobject_cast<MainWindow*>(w))
        {
            mainWindow = mainWin;
            connect(mainWin, SIGNAL(tick()), this, SLOT(repaint()));
            connect(mainWin, SIGNAL(accUpdated(int)), this, SLOT(updateVerticalSpeed()));
        }
}

void hprcPayloadGraph::updateVerticalSpeed() {
    if(mainWindow->m_currentData.altData.length() > 1) {
        verticalSpeedData.append(MainWindow::graphPoint{(mainWindow->m_currentData.altData[mainWindow->m_currentData.altData.length() - 1].value - mainWindow->m_currentData.altData[mainWindow->m_currentData.altData.length() - 2].value) / ((mainWindow->m_currentData.altData[mainWindow->m_currentData.altData.length() - 1].time - mainWindow->m_currentData.altData[mainWindow->m_currentData.altData.length() - 2].time) / 1000), mainWindow->m_currentData.altData[mainWindow->m_currentData.altData.length() - 1].time});
    }
}

void hprcGraph::mouseMoveEvent(QMouseEvent *e)
{
    m_mousePos = e->pos();

}

void hprcPayloadGraph::mouseMoveEvent(QMouseEvent *e)
{
    m_mousePos = e->pos();

}

hprcAlarmPanel::hprcAlarmPanel(QWidget *parent) :
    hprcDisplayWidget(parent)
{
    m_widgetType = HPRC_Alarm;
}

hprcClock::hprcClock(QWidget *parent) :
    hprcDisplayWidget(parent)
{
    // subscribe to any data that needs to cause an update
    foreach (QWidget *w, qApp->topLevelWidgets())
        if (MainWindow* mainWin = qobject_cast<MainWindow*>(w))
        {
            connect(mainWin, SIGNAL(tick()), this, SLOT(repaint()));
        }


    m_widgetType = HPRC_Clock;
}
hprcRocketVisualizer::hprcRocketVisualizer(QWidget *parent) :
    hprcDisplayWidget(parent)
{
    m_widgetType = HPRC_RocketVisual;
}

Qt3DCore::QEntity *createRocketScene();

hprcViewer::hprcViewer(QWidget *parent) :
    hprcDisplayWidget{parent}
{
    m_widgetType = HPRC_Viewer;

    Qt3DExtras::Qt3DWindow *view = new Qt3DExtras::Qt3DWindow;

    Qt3DCore::QEntity *rootEntity = new Qt3DCore::QEntity;

    // Create rocket mesh and entity objects
    Qt3DCore::QEntity *rocketEntity = new Qt3DCore::QEntity(rootEntity);
    Qt3DRender::QMesh *rocketMesh = new Qt3DRender::QMesh();

    // Load the rocket mesh
    rocketMesh->setSource(QUrl(QStringLiteral("qrc:/models/HPRC_rocket.stl")));

    // Set the material of the rocket
    Qt3DExtras::QDiffuseSpecularMaterial *rocketMaterial = new Qt3DExtras::QDiffuseSpecularMaterial(rootEntity);
    rocketMaterial->setDiffuse(QColor(255, 0, 0)); // Set the diffuse color (red in this case)
    rocketMaterial->setAmbient(QColor(100, 100, 100));
    rocketMaterial->setSpecular(QColor(255, 255, 255)); // Set the specular color (white)
    rocketMaterial->setShininess(10.0); // Set the shininess (adjust to control specular highlights)

    Qt3DCore::QTransform *rocketTransform = new Qt3DCore::QTransform;
    rocketTransform->setTranslation(QVector3D(0.0, m_rocketVertOffset, 0.0));

    // Scale the rocket to fit correctly on the screen
    rocketTransform->setScale3D(QVector3D(m_rocketScale, m_rocketScale, m_rocketScale));

    // Set the rotation of the rocket based on the input quaternion
    rocketTransform->setRotation(
        QQuaternion::fromEulerAngles(0.0f, 0.0f, 0.0f) * m_rocketOrientVertically);

    rocketEntity->addComponent(rocketMesh);
    rocketEntity->addComponent(rocketTransform);
    rocketEntity->addComponent(rocketMaterial);

    Qt3DCore::QEntity *scene = rootEntity;

    // Camera
    Qt3DRender::QCamera *camera = view->camera();
    camera->setPosition(QVector3D(0, 0, m_cameraDistToRocket));
    camera->setViewCenter(QVector3D(0, 0, 0));

    view->setRootEntity(scene);
    view->defaultFrameGraph()->setClearColor(QColor(QRgb(0x000000)));

    // Store the transform of the rocket
    m_rocketTransform = rocketTransform;

    // Store the 3d view
    m_view = view;

    // Store the rocket material
    m_rocketMaterial = rocketMaterial;

    QWidget *container = QWidget::createWindowContainer(view, this);

    container->setContentsMargins(0, 0, 0, 0);

    // Resize the 3D viewer to match the container widget
    m_view->resize(this->size());

    // Connect each instance of the widget to the orientation update signal
    foreach (QWidget *w, qApp->topLevelWidgets())
        if (MainWindow* mainWin = qobject_cast<MainWindow*>(w))
        {
            connect(mainWin, SIGNAL(orientUpdated(QQuaternion)), this, SLOT(repaint()));
        }
}

void hprcViewer::orientRocket(QQuaternion orientation) const {
    // Set the rotation of the rocket based on the input quaternion
    m_rocketTransform->setRotation(orientation * m_rocketOrientVertically);
}

void hprcViewer::updateColors(QColor panel, QColor highlight) const {
    // Adjust the rocket color
    m_rocketMaterial->setDiffuse(highlight);

    // Adjust the background color
    m_view->defaultFrameGraph()->setClearColor(panel);
}

hprcAirbrakes::hprcAirbrakes(QWidget* parent):
    hprcDisplayWidget(parent)
{
    // subscribe to any data that needs to cause an update
    foreach (QWidget *w, qApp->topLevelWidgets())
        if (MainWindow* mainWin = qobject_cast<MainWindow*>(w))
        {
            connect(mainWin, SIGNAL(desiredAirbrakesUpdated(float)), this, SLOT(repaint()));
            connect(mainWin, SIGNAL(currentAirbrakesUpdated(float)), this, SLOT(repaint()));
        }


    m_widgetType = HPRC_AIRBRAKES;
}
