#include "qt3dwidget.h"
#include "qt3dwidget_p.h"

#include <QDebug>
#include <QThread>
#include <QApplication>

#include <QSurfaceFormat>
#include <QOpenGLTexture>
#include <QOpenGLFunctions>
#include <QOpenGLFramebufferObject>
#include <Qt3DRender/private/qrenderaspect_p.h>
#include <Qt3DRender/private/abstractrenderer_p.h>

Qt3DWidgetPrivate::Qt3DWidgetPrivate()
    : m_aspectEngine(new Qt3DCore::QAspectEngine)
    , m_renderAspect(new Qt3DRender::QRenderAspect(Qt3DRender::QRenderAspect::Synchronous))
    , m_inputAspect(new Qt3DInput::QInputAspect)
    , m_logicAspect(new Qt3DLogic::QLogicAspect)
    , m_renderSettings(new Qt3DRender::QRenderSettings)
    , m_forwardRenderer(new Qt3DExtras::QForwardRenderer)
    , m_defaultCamera(new Qt3DRender::QCamera)
    , m_inputSettings(new Qt3DInput::QInputSettings)
    , m_frameAction(new Qt3DLogic::QFrameAction)
    , m_root(new Qt3DCore::QEntity)
    , m_userRoot(nullptr)
    , m_initialized(false) {
}

Qt3DWidget::Qt3DWidget(QWidget *parent)
    : QOpenGLWidget(parent)
    , d_ptr(new Qt3DWidgetPrivate) {
    Q_D(Qt3DWidget);

    d->m_aspectEngine->setRunMode(Qt3DCore::QAspectEngine::Manual);
    d->m_aspectEngine->registerAspect(d->m_renderAspect);
    d->m_aspectEngine->registerAspect(d->m_inputAspect);
    d->m_inputSettings->setEventSource(this);
    d->m_aspectEngine->registerAspect(d->m_logicAspect);

    Qt3DRender::QRenderAspectPrivate *dRenderAspect = static_cast<decltype(dRenderAspect)>
                    (Qt3DRender::QRenderAspectPrivate::get(d->m_renderAspect));
    Qt3DRender::Render::AbstractRenderer *renderer = dRenderAspect->m_renderer;
    // If we don't set the context here already we only get half (???) of the image
    // But we have to set it again in initializeGL otherwise we won't see anything
    renderer->setOpenGLContext(context());
    renderer->initialize();

    d->m_defaultCamera->setParent(d->m_forwardRenderer);
    d->m_forwardRenderer->setCamera(d->m_defaultCamera);
    d->m_forwardRenderer->setParent(d->m_renderSettings);
    d->m_renderSettings->setActiveFrameGraph(d->m_forwardRenderer);

    d->m_activeFrameGraph = d->m_forwardRenderer;
    d->m_forwardRenderer->setClearColor("white");
}

Qt3DWidget::~Qt3DWidget() {
    Q_D(Qt3DWidget);
    // Qt3D has a bug that when you set an externally managed QOpenGLContext it correctly detects this
    // and registers on the contexts aboutToBeDestroyed() signal to clear graphics resources. This causes
    // a segmentation fault because the QOpenGLWidget deletes the context which fires the signal but
    // at this point, Qt3D has been deconstructed already and can't receive the signal anymore.
    // Since the signal is connected to a lambda inside Qt3D, this connection is not removed when
    // Qt3D is deconstructed.
    context()->disconnect();
    Qt3DRender::QRenderAspectPrivate *dRenderAspect = static_cast<decltype(dRenderAspect)>
                    (Qt3DRender::QRenderAspectPrivate::get(d->m_renderAspect));
    Qt3DRender::Render::AbstractRenderer *renderer = dRenderAspect->m_renderer;
    // Qt3D acquires a semaphore internally when shutting down. Since Qt3D is connected to the
    // context's aboutToBeDestroyed() signal with a lambda which again clears the graphics resources
    // which acquires the same semaphore, everything is deadlocked. That's why we have to set this
    // null context here. Qt3D constructs a new context when passing a null pointer and everything
    // is fine since it doesn't register for the signal in this case (only happens when we
    // pass an existing context).
    renderer->setOpenGLContext(Q_NULLPTR);
    // We need to call initialize after setting the context to drive Qt3D to set everything up.
    // Only setting the context doesn't do anything.
    renderer->initialize();
    renderer->shutdown();
    // This stops the simulation loop - we actually driver everything manually but update the
    // engine using a timer and this prevents a crash.
    d->m_aspectEngine->setRootEntity(Qt3DCore::QEntityPtr());
    d->m_aspectEngine->unregisterAspect(d->m_renderAspect);
    delete d->m_renderAspect;
    delete d->m_aspectEngine;
}

// INIT!!!!!!!!!
void Qt3DWidget::initializeGL() {
    Q_D(Qt3DWidget);

    QOffscreenSurface *surface = (QOffscreenSurface*) context()->surface();
    ((Qt3DExtras::QForwardRenderer *) d->m_activeFrameGraph)->setSurface(surface);

    Qt3DRender::QRenderAspectPrivate *dRenderAspect = static_cast<decltype(dRenderAspect)>
                    (Qt3DRender::QRenderAspectPrivate::get(d->m_renderAspect));
    Qt3DRender::Render::AbstractRenderer *renderer = dRenderAspect->m_renderer;
    // If we don't set the context here again we obtain a black image only
    renderer->setOpenGLContext(context());
    renderer->initialize();

    d->m_root->addComponent(d->m_renderSettings);
    d->m_root->addComponent(d->m_inputSettings);
    d->m_root->addComponent(d->m_frameAction);
    //connect(d->m_frameAction, &Qt3DLogic::QFrameAction::triggered,
    //        this, &Qt3DWidget::paintGL);
    d->m_aspectEngine->setRootEntity(Qt3DCore::QEntityPtr(d->m_root));

    d->m_initialized = true;

    d->m_updateTimer.setInterval(20); // 50 fps
    connect(&d->m_updateTimer, &QTimer::timeout, this, &Qt3DWidget::paintGL);
    connect(&d->m_updateTimer, &QTimer::timeout, [d, this](){
        Qt3DExtras::QForwardRenderer * test = (Qt3DExtras::QForwardRenderer *) d->m_activeFrameGraph;
        QColor color = test->clearColor();
        int red = color.red() + d->colorDirection;
        int green = color.green() + d->colorDirection;
        int blue = color.blue() + d->colorDirection;
        test->setClearColor(QColor(red, green, blue));
        if (red == 0) {
            d->colorDirection = 1;
        } else if (red == 255) {
            d->colorDirection = -1;
        }
        update();
    });
    d->m_updateTimer.start();
}

void Qt3DWidget::resizeGL(int w, int h) {
    Q_D(Qt3DWidget);
    d->m_defaultCamera->setAspectRatio(w / (float) h);
}

void Qt3DWidget::paintGL() {
    Q_D(Qt3DWidget);
    d->m_aspectEngine->processFrame();

    Qt3DRender::QRenderAspectPrivate *dRenderAspect = static_cast<decltype(dRenderAspect)>
                    (Qt3DRender::QRenderAspectPrivate::get(d->m_renderAspect));
    Qt3DRender::Render::AbstractRenderer *renderer = dRenderAspect->m_renderer;
    if (renderer->shouldRender())
        renderer->doRender(true);
}

void Qt3DWidget::registerAspect(Qt3DCore::QAbstractAspect *aspect) {
    Q_D(Qt3DWidget);
    d->m_aspectEngine->registerAspect(aspect);
}

void Qt3DWidget::registerAspect(const QString &name) {
    Q_D(Qt3DWidget);
    d->m_aspectEngine->registerAspect(name);
}

void Qt3DWidget::setRootEntity(Qt3DCore::QEntity *root) {
    Q_D(Qt3DWidget);
    if (d->m_userRoot != root) {
        if (d->m_userRoot != nullptr)
            d->m_userRoot->setParent(static_cast<Qt3DCore::QNode*>(nullptr));
        if (root != nullptr)
            root->setParent(d->m_root);
        d->m_userRoot = root;
    }
}

void Qt3DWidget::setActiveFrameGraph(Qt3DRender::QFrameGraphNode *activeFrameGraph) {
    Q_D(Qt3DWidget);
    d->m_activeFrameGraph->setParent(static_cast<Qt3DCore::QNode*>(nullptr));
    d->m_activeFrameGraph = activeFrameGraph;
    d->m_activeFrameGraph->setParent(d->m_renderSettings);
}

Qt3DRender::QFrameGraphNode *Qt3DWidget::activeFrameGraph() const {
    Q_D(const Qt3DWidget);
    return d->m_activeFrameGraph;
}

Qt3DExtras::QForwardRenderer *Qt3DWidget::defaultFrameGraph() const {
    Q_D(const Qt3DWidget);
    return d->m_forwardRenderer;
}

Qt3DRender::QCamera *Qt3DWidget::camera() const {
    Q_D(const Qt3DWidget);
    return d->m_defaultCamera;
}

Qt3DRender::QRenderSettings *Qt3DWidget::renderSettings() const {
    Q_D(const Qt3DWidget);
    return d->m_renderSettings;
}

QSurface *Qt3DWidget::surface() const {
    return context()->surface();
}
