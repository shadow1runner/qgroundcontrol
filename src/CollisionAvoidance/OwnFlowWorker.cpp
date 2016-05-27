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
    // _converterThread.setObjectName("OwnFlowConverter");
    _ownFlowThread.setObjectName("OwnFlow");
    _grapherThread.setObjectName("OwnFlowGrapher");
    _ioThread.setObjectName("OwnFlow I/O");
    _uiFramePreparerThread.setObjectName("OwnFlow UI Frame Preparer");

    // _converter.moveToThread(&_converterThread);
    _ownFlow.moveToThread(&_ownFlowThread);
    _grapher.moveToThread(&_grapherThread);
    _framePersister.moveToThread(&_ioThread);
    _uiFramePreparer.moveToThread(&_uiFramePreparerThread);

    // converter -> ownflow
    connect(&_converter, &hw::Converter::imageConverted,
            &_ownFlow, &hw::OwnFlow::processImage
            ,Qt::BlockingQueuedConnection
           );

    // ownflow -> dependencies: UI Frame Preparer
    connect(&_ownFlow, &hw::OwnFlow::collisionLevelRatingReady,
            &_uiFramePreparer, &hw::UiFramePreparer::collisionLevelRatingReady);

    connect(&_ownFlow, &hw::OwnFlow::frameSkipped,
            &_uiFramePreparer, &hw::UiFramePreparer::badFrame);

    connect(&_ownFlow, &hw::OwnFlow::opticalFlowChanged,
            &_uiFramePreparer, &hw::UiFramePreparer::opticalFlowReady);
    
    connect(&_ownFlow, &hw::OwnFlow::histogramChanged,
            &_uiFramePreparer, &hw::UiFramePreparer::histogramReady);

    // ownflow -> dependencies: I/O connections, frame persistence
    connect(_frameGrabber, &hw::BufferedFrameGrabber::newRawFrame,
            &_framePersister, &FramePersister::rawFrameReady);

    connect(&_ownFlow, &hw::OwnFlow::collisionLevelRatingReady,
            &_framePersister, &FramePersister::collisionLevelRatingReady);

    connect(&_ownFlow, &hw::OwnFlow::frameSkipped,
            &_framePersister, &FramePersister::badFrameReady);

    connect(&_ownFlow, &hw::OwnFlow::opticalFlowChanged,
            &_framePersister, &FramePersister::opticalFlowReady);
    
    connect(&_ownFlow, &hw::OwnFlow::histogramChanged,
            &_framePersister, &FramePersister::histogramReady);
    
    // UI -> I/O
    connect(&_uiFramePreparer, &hw::UiFramePreparer::uiFrameReady,
            &_framePersister, &FramePersister::uiFrameReady);
    
    // UI Frame Preparer -> UI Thread
    connect(&_uiFramePreparer, &hw::UiFramePreparer::qtUiFrameReady,
             _collisionAvoidanceDataProvider, &CollisionAvoidanceDataProvider::qtUiFrameReady);
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
    // _converterThread.start();
    _grapherThread.start();
    _ownFlowThread.start();
    _uiFramePreparerThread.start();

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

    // _converterThread.quit();
    // _converterThread.wait();

    _ownFlowThread.quit();
    _ownFlowThread.wait();

    _grapherThread.quit();
    _grapherThread.wait();

    _ioThread.quit();
    _ioThread.wait();

    _uiFramePreparerThread.quit();
    _uiFramePreparerThread.wait();
}

void OwnFlowWorker::reset()
{
    qDebug() << "Resetting OwnFlowWorker on Thread" << QThread::currentThreadId();
    pause();
    QThread::msleep(100);
    _frameGrabber->reset();
    QMetaObject::invokeMethod(&_ownFlow, "reset");
}

void OwnFlowWorker::_collisionImmanent(const cv::Mat& frame, unsigned long long frameNumber, std::shared_ptr<cv::Point2i> foeFiltered, std::shared_ptr<hw::FocusOfExpansionDto> foe, const hw::CollisionLevel collisionLevel)
{
    Q_UNUSED(frame);
    Q_UNUSED(frameNumber);
    Q_UNUSED(foeFiltered);
    Q_UNUSED(foe);
    Q_UNUSED(collisionLevel);

    qDebug() << "Pausing because of received `collisionImmanent` event";
    pause();
}
