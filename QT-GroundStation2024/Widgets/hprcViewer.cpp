#include "hprcViewer.h"

#include "Windows/mainwindow.h"

#include <QApplication>

Qt3DCore::QEntity* createRocketScene();

hprcViewer::hprcViewer(QWidget* parent) :
    hprcDisplayWidget{ parent }
{
    m_widgetType = HPRC_Viewer;

    Qt3DExtras::Qt3DWindow* view = new Qt3DExtras::Qt3DWindow;

    Qt3DCore::QEntity* rootEntity = new Qt3DCore::QEntity;

    // Create rocket mesh and entity objects
    Qt3DCore::QEntity* rocketEntity = new Qt3DCore::QEntity(rootEntity);
    Qt3DRender::QMesh* rocketMesh = new Qt3DRender::QMesh();

    // Load the rocket mesh
    rocketMesh->setSource(QUrl(QStringLiteral("qrc:/Models/HPRC_rocket.stl")));

    // Set the material of the rocket
    Qt3DExtras::QDiffuseSpecularMaterial* rocketMaterial = new Qt3DExtras::QDiffuseSpecularMaterial(rootEntity);
    rocketMaterial->setDiffuse(QColor(255, 0, 0)); // Set the diffuse color (red in this case)
    rocketMaterial->setAmbient(QColor(100, 100, 100));
    rocketMaterial->setSpecular(QColor(255, 255, 255)); // Set the specular color (white)
    rocketMaterial->setShininess(10.0); // Set the shininess (adjust to control specular highlights)

    Qt3DCore::QTransform* rocketTransform = new Qt3DCore::QTransform;
    rocketTransform->setTranslation(QVector3D(0.0, m_rocketVertOffset, 0.0));

    // Scale the rocket to fit correctly on the screen
    rocketTransform->setScale3D(QVector3D(m_rocketScale, m_rocketScale, m_rocketScale));

    // Set the rotation of the rocket based on the input quaternion
    rocketTransform->setRotation(
        QQuaternion::fromEulerAngles(0.0f, 0.0f, 0.0f) * m_rocketOrientVertically);

    rocketEntity->addComponent(rocketMesh);
    rocketEntity->addComponent(rocketTransform);
    rocketEntity->addComponent(rocketMaterial);

    Qt3DCore::QEntity* scene = rootEntity;

    // Camera
    Qt3DRender::QCamera* camera = view->camera();
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

    QWidget* container = QWidget::createWindowContainer(view, this);

    container->setContentsMargins(0, 0, 0, 0);

    // Resize the 3D viewer to match the container widget
    m_view->resize(this->size());

    // Connect each instance of the widget to the orientation update signal
    foreach(QWidget * w, qApp->topLevelWidgets())
        if (MainWindow* mainWin = qobject_cast<MainWindow*>(w))
        {
            connect(mainWin, SIGNAL(orientationUpdated()), this, SLOT(repaint()));
            //            connect(mainWin, SIGNAL(orientUpdated(QQuaternion)), this, SLOT(repaint()));
        }
}

void hprcViewer::orientRocket(QQuaternion orientation) const {
    // Set the rotation of the rocket based on the input quaternion
    m_rocketTransform->setRotation(orientation * m_rocketOrientVertically);
}

void hprcViewer::updateColors(QColor panel, QColor highlight) const {
    // Adjust the rocket color
//    m_rocketMaterial->setDiffuse(highlight);

    // Adjust the background color
    m_view->defaultFrameGraph()->setClearColor(panel);
}