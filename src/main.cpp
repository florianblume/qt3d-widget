#include "mainwindow.h"
#include "qt3dwidget.h"

#include <QApplication>
#include <QDebug>

#include <Qt3DCore/QEntity>
#include <Qt3DExtras/QTorusMesh>
#include <Qt3DExtras/QPhongMaterial>
#include <Qt3DExtras/QOrbitCameraController>

int main(int argc, char *argv[]) {
    QApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
    QApplication a(argc, argv);
    // The order of the following calls is important otherwise we won't see anything.
    MainWindow w;
    w.show();
    Qt3DWidget *widget = new Qt3DWidget();
    widget->show();
    w.setCentralWidget(widget);

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

    widget->setRootEntity(entity);
    widget->camera()->viewAll();

    return a.exec();
}
