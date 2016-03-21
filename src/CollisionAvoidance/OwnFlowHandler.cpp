#include "OwnFlowHandler.h"

#include <QSettings>

#include "QGCToolbox.h"

OwnFlowHandler::OwnFlowHandler(QGCApplication* app)
    : QGCTool(app)
    , _collisionAvoidanceDataProvider(nullptr)
{
	loadSettings();
}

OwnFlowHandler::~OwnFlowHandler()
{
	storeSettings();
}

void OwnFlowHandler::setToolbox(QGCToolbox* toolbox)
{
	QGCTool::setToolbox(toolbox);
    _collisionAvoidanceDataProvider = toolbox->collisionAvoidanceDataProvider();
}

void OwnFlowHandler::loadSettings()
{
    // Load defaults from settings
    QSettings settings;
    settings.beginGroup("QGC_COLLISION_AVOIDANCE");
    FILENAME = settings.value("FILENAME", "res/rl/go5.mp4").toString().toStdString();
    OUTPUT_DIR = settings.value("OUTPUT_DIR", "res/out/RandomCollider/cpp/frame_").toString().toStdString();
    SUBSAMPLE_AMOUNT = settings.value("SUBSAMPLE_AMOUNT", 2).toInt();
    PARTICLES = settings.value("PARTICLES", 20000).toInt();
    WINDOW_SIZE = settings.value("WINDOW_SIZE", 5).toInt();
    INLIER_PROPORTION_THRESHOLD = settings.value("INLIER_PROPORTION_THRESHOLD", 0.01).toDouble();
    DIVERGENCE_PATCHSIZE = settings.value("DIVERGENCE_PATCHSIZE", 20).toInt();
    DIVERGENCE_THRESHOLD = settings.value("DIVERGENCE_THRESHOLD", 0.08).toDouble();
    settings.endGroup();
}

void OwnFlowHandler::storeSettings()
{
    // Store settings
    QSettings settings;
    settings.setValue("FILENAME", QString::fromStdString(FILENAME));
    settings.setValue("OUTPUT_DIR", QString::fromStdString(OUTPUT_DIR));
    settings.setValue("SUBSAMPLE_AMOUNT", SUBSAMPLE_AMOUNT);
    settings.setValue("PARTICLES", PARTICLES);
    settings.setValue("WINDOW_SIZE", WINDOW_SIZE);
    settings.setValue("INLIER_PROPORTION_THRESHOLD", INLIER_PROPORTION_THRESHOLD);
    settings.setValue("DIVERGENCE_PATCHSIZE", DIVERGENCE_PATCHSIZE);
    settings.setValue("DIVERGENCE_THRESHOLD", DIVERGENCE_THRESHOLD);
    settings.endGroup();
}
