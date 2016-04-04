#include "OwnFlowHandler.h"
#include "OwnFlowWorker.h"

#include <QSettings>
#include <QThread>

#include "QGCToolbox.h"
#include "QtHelper.h"

OwnFlowHandler::OwnFlowHandler(QGCApplication* app)
    : QGCTool(app)
{
	loadSettings();
    QtHelper::registerMetaTypes();

    _ownFlowWorker = new OwnFlowWorker(FILENAME, this);
    _ownFlowWorker->moveToThread(&_ownFlowWorkerThread);

    _converter = new hw::Converter(SUBSAMPLE_AMOUNT);
    _converter->moveToThread(&_converterThread);

    _ownFlow = new hw::OwnFlow(PARTICLES, WINDOW_SIZE);
    _ownFlow->moveToThread(&_ownFlowThread);
}

OwnFlowHandler::~OwnFlowHandler()
{
	stop();
    storeSettings();
    
    delete _converter;
    delete _ownFlow;
    delete _ownFlowWorker;
}

void OwnFlowHandler::setToolbox(QGCToolbox* toolbox)
{
	QGCTool::setToolbox(toolbox);
    _collisionAvoidanceDataProvider = toolbox->collisionAvoidanceDataProvider();

    QObject::connect(_converter, &hw::Converter::imageConverted,
                     _ownFlow, &hw::OwnFlow::processImage,
                     // Qt::DirectConnection); // can't be use - IPC
                     // Qt::QueuedConnection);
//                      Qt::AutoConnection); // does NOT work, most likely because OwnFlow gets overwhelmed
                     Qt::BlockingQueuedConnection); // works
                     // Qt::UniqueConnection);
                     // https://doc.qt.io/qt-5/qt.html#ConnectionType-enum
                     // https://wiki.qt.io/Threads_Events_QObjects


    QObject::connect(_ownFlow, &hw::OwnFlow::foeChanged,
                     _collisionAvoidanceDataProvider, &CollisionAvoidanceDataProvider::foeReady
                     );//Qt::BlockingQueuedConnection);

    QObject::connect(_ownFlow, &hw::OwnFlow::opticalFlowChanged,
                     _collisionAvoidanceDataProvider, &CollisionAvoidanceDataProvider::opticalFlowReady);
    
    QObject::connect(_ownFlow, &hw::OwnFlow::histogramChanged,
                     _collisionAvoidanceDataProvider, &CollisionAvoidanceDataProvider::histogramReady);
}

void OwnFlowHandler::start()
{
    qDebug() << "Starting OwnFlowHandler on Thread" << QThread::currentThreadId();
    _converterThread.start();
    _ownFlowThread.start();
    _ownFlowWorkerThread.start();

    emit startTriggered();
}

void OwnFlowHandler::stop()
{
    qDebug() << "Stopping OwnFlowHandler on Thread" << QThread::currentThreadId();

    _ownFlowWorkerThread.quit();
    _ownFlowWorkerThread.wait();

    _converterThread.quit();
    _converterThread.wait();

    _ownFlowThread.quit();
    _ownFlowThread.wait();

    emit stopTriggered();
}


void OwnFlowHandler::loadSettings()
{
    // Load defaults from settings
    QSettings settings;
    settings.beginGroup("QGC_COLLISION_AVOIDANCE");
    FILENAME = settings.value("FILENAME", "CollisionAvoidance/res/rl/go5.mp4").toString().toStdString();
    OUTPUT_DIR = settings.value("OUTPUT_DIR", "CollisionAvoidance/res/out/RandomCollider/cpp/frame_").toString().toStdString();
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
