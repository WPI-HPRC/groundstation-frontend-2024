#include "mainwindow.h"
#include "styles.h"

#include <QApplication>

// All 3d testing stuff
#include <Qt3DCore/QEntity>
#include <Qt3DRender/QMesh>
#include <Qt3DRender/QGeometryRenderer>
#include <Qt3DRender/QCamera>
#include <Qt3DRender/QCameraLens>
#include <Qt3DRender/QDirectionalLight>
#include <Qt3DCore/QTransform>
#include <Qt3DCore/QAspectEngine>

#include <Qt3DInput/QInputAspect>

#include <Qt3DRender/QRenderAspect>
#include <Qt3DRender/QGeometryRenderer>
#include <Qt3DExtras/QForwardRenderer>
#include <Qt3DExtras/QDiffuseSpecularMaterial>
#include <Qt3DExtras/QSphereMesh>
#include <Qt3DExtras/QCylinderMesh>
#include <Qt3DExtras/QTorusMesh>
#include <Qt3DExtras/QOrbitCameraController>
#include <Qt3DExtras/Qt3DWindow>
#include <QPropertyAnimation>

Qt3DCore::QEntity *createRocketScene();

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyle(new HPRCDarkStyle(nullptr));
    MainWindow w;
    a.setStyle(new HPRCDarkStyle(w.getCurrentData()));
    w.centralWidget()->setBackgroundRole(QPalette::Window);
    w.show();
    w.showMaximized();
    w.update();


    // 3D TESTING
    Qt3DExtras::Qt3DWindow view;

    Qt3DCore::QEntity *scene = createRocketScene();

    // Camera
    Qt3DRender::QCamera *camera = view.camera();
    camera->setPosition(QVector3D(0, 0, 40.0f));
    camera->setViewCenter(QVector3D(0, 0, 0));

    view.setRootEntity(scene);
    view.defaultFrameGraph()->setClearColor(QColor(QRgb(0x607D8B))); // set the background color

    // dark background: #141414
    // light background: #607D8B

    // dark highlight: 175, 40, 58
    // light highlight: 175, 40, 58

    view.show();

    return a.exec();
}

Qt3DCore::QEntity *createRocketScene()
{
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

    return rootEntity;
}
