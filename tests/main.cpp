#include "mainwindow.h"
#include "qt3dwidget.h"

#include <QApplication>
#include <QDebug>
#include <QPushButton>
#include <QTimer>
#include <QSurfaceFormat>

#include <Qt3DCore/QEntity>
#include <Qt3DExtras/QTorusMesh>
#include <Qt3DExtras/QPhongMaterial>
#include <Qt3DExtras/QOrbitCameraController>
#include <Qt3DCore/QTransform>

int main(int argc, char *argv[]) {
    QSurfaceFormat format;
    format.setVersion(4, 3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setDepthBufferSize(24);
    format.setSamples(8);
    format.setStencilBufferSize(8);
    QSurfaceFormat::setDefaultFormat(format);

    // We need this, otherwise the application hangs
    QApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
    QApplication a(argc, argv);
    // The order of the following calls is important otherwise we won't see anything.
    MainWindow w;
    w.show();
    Qt3DWidget *widget = new Qt3DWidget();
    widget->show();
    w.setCentralWidget(widget);

    QPushButton *button = new QPushButton(widget);
    button->setGeometry(QRect(30, 30, 100, 100));
    button->setText("Press me!");

    // Test scene
    Qt3DExtras::QTorusMesh *mesh = new Qt3DExtras::QTorusMesh();
    mesh->setRings(100);
    mesh->setSlices(100);
    mesh->setRadius(3);
    Qt3DExtras::QPhongMaterial *material = new Qt3DExtras::QPhongMaterial();
    material->setAmbient("red");
    Qt3DCore::QEntity *entity = new Qt3DCore::QEntity();
    entity->addComponent(mesh);
    entity->addComponent(material);
    Qt3DExtras::QOrbitCameraController *cameraController = new Qt3DExtras::QOrbitCameraController(entity);
    cameraController->setCamera(widget->camera());
    Qt3DCore::QTransform *transform = new Qt3DCore::QTransform(entity);
    entity->addComponent(transform);
    QTimer timer;
    timer.setInterval(10);
    QObject::connect(&timer, &QTimer::timeout, [transform](){
        //transform->setRotationX(transform->rotationX() + 1.f);
        //transform->setRotationY(transform->rotationY() + 1.f);
    });
    timer.start();

    widget->setRootEntity(entity);
    widget->camera()->viewAll();

    return a.exec();
}
