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
#include <Qt3DRender/QDirectionalLight>
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

void hprcDisplayWidget::updateFilled(int input)
{

    QPropertyAnimation *animation = new QPropertyAnimation(this, "filledPercent");
    animation->setDuration(400);
    animation->setEndValue(input);
    animation->start();
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
            connect(mainWin, SIGNAL(altUpdated(int)), this, SLOT(updateFilled(int)));
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

void hprcGraph::mouseMoveEvent(QMouseEvent *e)
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

Qt3DCore::QEntity *createRocketScene();

hprcViewer::hprcViewer(QWidget *parent) :
    hprcDisplayWidget{parent}
{
    m_widgetType = HPRC_Viewer;

    Qt3DExtras::Qt3DWindow *view = new Qt3DExtras::Qt3DWindow;

    Qt3DCore::QEntity *rootEntity = new Qt3DCore::QEntity;

    // Create an environment light
    Qt3DRender::QDirectionalLight* light = new Qt3DRender::QDirectionalLight(rootEntity);
    light->setIntensity(0);

    // Set the intensity of the environment light (adjust as needed)

    // Create rocket mesh and entity objects
    Qt3DCore::QEntity *rocketEntity = new Qt3DCore::QEntity(rootEntity);
    Qt3DRender::QMesh *rocketMesh = new Qt3DRender::QMesh();

    // Load the rocket mesh
    rocketMesh->setSource(QUrl(QStringLiteral("qrc:/models/HPRC_rocket.stl")));

    // Set the material of the rocket
    Qt3DExtras::QDiffuseSpecularMaterial *rocketMaterial = new Qt3DExtras::QDiffuseSpecularMaterial(rootEntity);
    rocketMaterial->setDiffuse(QColor(255, 0, 0)); // Set the diffuse color (red in this case)
    rocketMaterial->setSpecular(QColor(255, 255, 255)); // Set the specular color (white)
    rocketMaterial->setShininess(0.0); // Set the shininess (adjust to control specular highlights)
    //    rocketMaterial->setDiffuseColor(Qt::red);
    //    geometry->rocketMaterial(material);

    Qt3DCore::QTransform *rocketTransform = new Qt3DCore::QTransform;
    rocketTransform->setTranslation(QVector3D(0.0, 0.0, 0.0));

    // Scale the rocket to fit correctly on the screen
    rocketTransform->setScale3D(QVector3D(0.15, 0.15, 0.15));

    // Set the rotation of the rocket based on the input quaternion
    // Note: the -90 is there to orient the rocket correctly
    // since the STL will face out of the screen by default
    rocketTransform->setRotation(
        QQuaternion::fromEulerAngles(0.0f, 0.0f, 0.0f) * QQuaternion::fromEulerAngles(-90.0f, 0.0f, 0.0f)); // pitch, yaw, roll


    rocketEntity->addComponent(rocketMesh);
    rocketEntity->addComponent(rocketTransform);
    rocketEntity->addComponent(rocketMaterial);

    Qt3DCore::QEntity *scene = rootEntity;

    // Camera
    Qt3DRender::QCamera *camera = view->camera();
    camera->setPosition(QVector3D(0, 0, 40.0f));
    camera->setViewCenter(QVector3D(0, 0, 0));

    view->setRootEntity(scene);
    view->defaultFrameGraph()->setClearColor(QColor(QRgb(0x607D8B))); // set the background color

    // Store the transform of the rocket
    m_rocketTransform = rocketTransform;

//    QWidget *container = QWidget::createWindowContainer(view, this);
    QWidget::createWindowContainer(view, this);

    foreach (QWidget *w, qApp->topLevelWidgets())
        if (MainWindow* mainWin = qobject_cast<MainWindow*>(w))
        {
//            connect(mainWin, SIGNAL(orientUpdated(QQuaternion)), this, SLOT(repaint()));
        }
}
