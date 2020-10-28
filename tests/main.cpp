#include "mainwindow.h"
#include "qt3dwidget.h"

#include <QApplication>
#include <QDebug>
#include <QTimer>
#include <QPushButton>

#include <Qt3DCore/QEntity>
#include <Qt3DCore/QTransform>
#include <Qt3DExtras/QTorusMesh>
#include <Qt3DExtras/QPhongMaterial>
#include <Qt3DExtras/QOrbitCameraController>
#include <Qt3DExtras/QForwardRenderer>

int main(int argc, char *argv[]) {
    QApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    Qt3DWidget *widget = new Qt3DWidget(&w);
    w.setCentralWidget(widget);
    QPushButton *button = new QPushButton;
    button->setText("Press me!");
    button->setParent(widget);
    button->setGeometry(15, 15, 100, 50);

    Qt3DExtras::QTorusMesh *mesh = new Qt3DExtras::QTorusMesh();
    mesh->setRings(100);
    mesh->setSlices(100);
    mesh->setMinorRadius(0.5);
    mesh->setRadius(1);
    Qt3DExtras::QPhongMaterial *material = new Qt3DExtras::QPhongMaterial();
    material->setAmbient("red");
    Qt3DCore::QTransform *transform = new Qt3DCore::QTransform();
    Qt3DCore::QEntity *entity = new Qt3DCore::QEntity();
    entity->addComponent(mesh);
    entity->addComponent(material);
    entity->addComponent(transform);
    Qt3DExtras::QOrbitCameraController *cameraController = new Qt3DExtras::QOrbitCameraController(entity);
    cameraController->setCamera(widget->camera());

    Qt3DExtras::QForwardRenderer *forwardRenderer = new Qt3DExtras::QForwardRenderer;
    forwardRenderer->setCamera(widget->camera());
    forwardRenderer->setClearColor(Qt::black);
    widget->setActiveFrameGraph(forwardRenderer);

    QTimer animationTimer;
    animationTimer.setInterval(10);
    QObject::connect(&animationTimer, &QTimer::timeout, [transform](){
        transform->setRotationX(transform->rotationX() + 1);
    });
    animationTimer.start();

    widget->setRootEntity(entity);
    widget->camera()->setPosition(QVector3D(0, 0, 10));

    return a.exec();
}
