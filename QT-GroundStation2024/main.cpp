#include "mainwindow.h"
#include "styles.h"

#include <QApplication>

// All 3d testing stuff
#include <Qt3DCore/QEntity>
#include <Qt3DRender/QMesh>
#include <Qt3DRender/QGeometryRenderer>
#include <Qt3DRender/QCamera>
#include <Qt3DRender/QCameraLens>
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

Qt3DCore::QEntity *createScene();

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

    Qt3DCore::QEntity *scene = createScene();

    // Camera
    Qt3DRender::QCamera *camera = view.camera();
//    camera->lens()->setPerspectiveProjection(45.0f, 16.0f/9.0f, 0.1f, 1000.0f);
//    camera->setPosition(QVector3D(0, 0, 40.0f));
    camera->setPosition(QVector3D(0, 0, 40.0f));
    camera->setViewCenter(QVector3D(0, 0, 0));

    // For camera controls
//    Qt3DExtras::QOrbitCameraController *camController = new Qt3DExtras::QOrbitCameraController(scene);
//    camController->setLinearSpeed( 50.0f );
//    camController->setLookSpeed( 180.0f );
//    camController->setCamera(camera);

    view.setRootEntity(scene);
    view.show();



    return a.exec();
}

Qt3DCore::QEntity *createScene()
{
    Qt3DCore::QEntity *rootEntity = new Qt3DCore::QEntity;


    // LOAD ROCKET
    // Create a mesh object.

    Qt3DCore::QEntity *rocketEntity = new Qt3DCore::QEntity(rootEntity);
    Qt3DRender::QMesh *rocketMesh = new Qt3DRender::QMesh();

    // Load the rocket mesh
    rocketMesh->setSource(QUrl(QStringLiteral("qrc:/models/HPRC_rocket.stl")));

    // Set the material of the rocket
    Qt3DRender::QMaterial *rocketMaterial = new Qt3DExtras::QDiffuseSpecularMaterial(rootEntity);
//    rocketMaterial->setDiffuseColor(Qt::red);
//    geometry->rocketMaterial(material);

    Qt3DCore::QTransform *rocketTransform = new Qt3DCore::QTransform;

    // Anchor the rocket correctly
    rocketTransform->setScale3D(QVector3D(0.15, 0.15, 0.15));
    // the rocket model is massive, I'd like to know the size and how to calculate how
    // to make everything fit correctly programmatically rather than guessing based on size
    rocketTransform->setTranslation(QVector3D(0, 0, 0)); // these numbers are found experimentally, figure out how to calculate them
    // rocketTransform->setTranslation(QVector3D(0, -1, 0));

    // Set the rotation of the geometry object.
    rocketTransform->setRotation(QQuaternion::fromEulerAngles(0.0f, 0.0f, 0.0f) * QQuaternion::fromEulerAngles(-90.0f, 0.0f, 0.0f)); // pitch, yaw, roll
//    rocketTransform->setRotation(QQuaternion::); // pitch, yaw, roll
//    rocketTransform->setRotation(QQuaternion::fromEulerAngles(-90.0f, 0.0f, 0.0f)); // pitch, yaw, roll

    rocketEntity->addComponent(rocketMesh);
    rocketEntity->addComponent(rocketTransform);
    rocketEntity->addComponent(rocketMaterial);


    return rootEntity;
}
