#ifndef QT3DWIDGET_P_H
#define QT3DWIDGET_P_H

#include "qt3dwidget.h"

#include <QObject>
#include <QOpenGLContext>
#include <QTimer>

#include <Qt3DRender/private/qt3drender_global_p.h>
#include <Qt3DCore/QAspectEngine>
#include <Qt3DRender/QRenderAspect>
#include <Qt3DInput/QInputAspect>
#include <Qt3DLogic/QLogicAspect>
#include <Qt3DRender/QRenderSettings>
#include <Qt3DExtras/QForwardRenderer>
#include <Qt3DRender/QCamera>
#include <Qt3DInput/QInputSettings>
#include <Qt3DRender/private/abstractrenderer_p.h>

class Qt3DWidgetPrivate : public QObject
{
public:
    Qt3DWidgetPrivate();

    Qt3DCore::QAspectEngine *m_aspectEngine;

    // Aspects
    Qt3DRender::QRenderAspect *m_renderAspect;
    Qt3DInput::QInputAspect *m_inputAspect;
    Qt3DLogic::QLogicAspect *m_logicAspect;

    Qt3DRender::Render::AbstractRenderer *m_renderer;

    // Renderer configuration
    Qt3DRender::QRenderSettings *m_renderSettings;
    Qt3DExtras::QForwardRenderer *m_forwardRenderer;
    Qt3DRender::QCamera *m_defaultCamera;
    Qt3DRender::QFrameGraphNode *m_activeFrameGraph;

    // Input configuration
    Qt3DInput::QInputSettings *m_inputSettings;

    // Scene
    Qt3DCore::QEntity *m_root;
    Qt3DCore::QEntity *m_userRoot;

    QTimer m_updateTimer;

    bool m_initialized;
};

#endif // QT3DWIDGET_P_H
