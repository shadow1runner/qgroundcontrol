#include "OwnFlowWorker.h"
#include "OwnFlowHandler.h"
#include "OwnFlowGrapher.h"

#include "QGCToolbox.h"

#include <QThread>

OwnFlowWorker::OwnFlowWorker(CollisionAvoidanceSettings& settings, QGCToolbox* toolbox)
    : QObject(NULL)
	, _settings(settings)
    , _collisionAvoidanceDataProvider(toolbox->collisionAvoidanceDataProvider())
    , _isPaused(false)
    , _frameGrabber(new hw::BufferedFrameGrabber(settings, 1, [](cv::Mat input) {return input;}, this))
    , _converter(settings)
    , _ownFlow(settings)
    , _grapher(&_ownFlow, toolbox)
    , _framePersister(settings)
{ 
    _converterThread.setObjectName("OwnFlowConverter");
    _ownFlowThread.setObjectName("OwnFlow");
    _grapherThread.setObjectName("OwnFlowGrapher");
    _ioThread.setObjectName("OwnFlow I/O");

    _converter.moveToThread(&_converterThread);
    _ownFlow.moveToThread(&_ownFlowThread);
    _grapher.moveToThread(&_grapherThread);
    _framePersister.moveToThread(&_ioThread);

    connect(&_converter, &hw::Converter::imageConverted,
            &_ownFlow, &hw::OwnFlow::processImage
            ,Qt::BlockingQueuedConnection
           );

    connect(&_ownFlow, &hw::OwnFlow::foeChanged,
             _collisionAvoidanceDataProvider, &CollisionAvoidanceDataProvider::foeReady);

    connect(&_ownFlow, &hw::OwnFlow::frameSkipped,
             _collisionAvoidanceDataProvider, &CollisionAvoidanceDataProvider::badFrame);

    connect(&_ownFlow, &hw::OwnFlow::opticalFlowChanged,
             _collisionAvoidanceDataProvider, &CollisionAvoidanceDataProvider::opticalFlowReady);
    
    connect(&_ownFlow, &hw::OwnFlow::histogramChanged,
             _collisionAvoidanceDataProvider, &CollisionAvoidanceDataProvider::histogramReady);

    // I/O connections
    connect(_frameGrabber, &hw::BufferedFrameGrabber::newRawFrame,
            &_framePersister, &FramePersister::rawFrameReady);

    connect(&_ownFlow, &hw::OwnFlow::foeChanged,
            &_framePersister, &FramePersister::foeFrameReady);

    connect(&_ownFlow, &hw::OwnFlow::frameSkipped,
            &_framePersister, &FramePersister::badFrameReady);

    connect(&_ownFlow, &hw::OwnFlow::opticalFlowChanged,
            &_framePersister, &FramePersister::opticalFlowReady);
    
    connect(&_ownFlow, &hw::OwnFlow::histogramChanged,
            &_framePersister, &FramePersister::histogramReady);
    
    connect(_collisionAvoidanceDataProvider, &CollisionAvoidanceDataProvider::uiFrameReady,
            &_framePersister, &FramePersister::uiFrameReady);
}

OwnFlowWorker::~OwnFlowWorker() {
    delete _frameGrabber;
}

hw::OwnFlow* OwnFlowWorker::ownFlow() {
    return &_ownFlow;
}

OwnFlowGrapher* OwnFlowWorker::ownFlowGrapher() {
    return &_grapher;
}

void OwnFlowWorker::start() 
{
    qDebug() << "Starting OwnFlowWorker on Thread" << QThread::currentThreadId();

    _ioThread.start();
    _converterThread.start();
    _grapherThread.start();
    _ownFlowThread.start();

    _isPaused = false;
    emit isPausedChanged(_isPaused);

    while(!_isPaused && _frameGrabber->has_next()) {
        auto currentFrame = _frameGrabber->next();
        _converter.convertImage(currentFrame);
	}

    reset();
}

void OwnFlowWorker::pause() 
{	
    qDebug() << "Pausing OwnFlowWorker on Thread" << QThread::currentThreadId();
    // there might be multiple writers, still aquiring a lock in front of each frame is not really necessary as long as the value gets set in the end (and thus the worker stops/pauses)
    _isPaused = true;
    emit isPausedChanged(_isPaused);
}

void OwnFlowWorker::stop() 
{
    pause();

    _converterThread.quit();
    _converterThread.wait();

    _ownFlowThread.quit();
    _ownFlowThread.wait();

    _grapherThread.quit();
    _grapherThread.wait();

    _ioThread.quit();
    _ioThread.wait();
}

void OwnFlowWorker::reset()
{
    qDebug() << "Resetting OwnFlowWorker on Thread" << QThread::currentThreadId();
    pause();
    QThread::msleep(100);
    _frameGrabber->reset();
}

void OwnFlowWorker::_collisionImmanent(const cv::Mat& frame, std::shared_ptr<hw::FocusOfExpansionDto> foeFiltered, std::shared_ptr<hw::FocusOfExpansionDto> foeMeasured, std::shared_ptr<hw::Divergence> divergence)
{
    Q_UNUSED(frame);
    Q_UNUSED(foeFiltered);
    Q_UNUSED(foeMeasured);
    Q_UNUSED(divergence);

    pause();
}
