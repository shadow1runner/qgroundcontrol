#include "OwnFlowWorker.h"
#include "OwnFlowHandler.h"
#include "OwnFlowGrapher.h"

#include "QGCToolbox.h"

OwnFlowWorker::OwnFlowWorker(CollisionAvoidanceSettings& settings, QGCToolbox* toolbox)
    : QObject(NULL)
	, _settings(settings)
    , _collisionAvoidanceDataProvider(toolbox->collisionAvoidanceDataProvider())
    , _isPaused(false)
    , _converter(settings)
    , _ownFlow(settings)
    , _grapher(&_ownFlow, toolbox)
{ 
    _converterThread.setObjectName("OwnFlowConverter");
    _ownFlowThread.setObjectName("OwnFlow");
    _grapherThread.setObjectName("OwnFlowGrapher");

    _converter.moveToThread(&_converterThread);
    _ownFlow.moveToThread(&_ownFlowThread);
    _grapher.moveToThread(&_grapherThread);

    if(settings.getUseRecordedVideoInsteadOfDevice()) 
        _frameGrabber = new hw::BufferedFrameGrabber(settings.getFileName().toStdString(), 1, [](cv::Mat input) {return input;});
    else
        _frameGrabber = new hw::BufferedFrameGrabber(settings.getDevice(), 1, [](cv::Mat input) {return input;});

    connect(&_converter, &hw::Converter::imageConverted,
            &_ownFlow, &hw::OwnFlow::processImage
            ,Qt::BlockingQueuedConnection
           );

    connect(&_ownFlow, &hw::OwnFlow::foeChanged,
             _collisionAvoidanceDataProvider, &CollisionAvoidanceDataProvider::foeReady
           );//Qt::BlockingQueuedConnection);

    connect(&_ownFlow, &hw::OwnFlow::frameSkipped,
             _collisionAvoidanceDataProvider, &CollisionAvoidanceDataProvider::badFrame
           );

    connect(&_ownFlow, &hw::OwnFlow::opticalFlowChanged,
             _collisionAvoidanceDataProvider, &CollisionAvoidanceDataProvider::opticalFlowReady);
    
    connect(&_ownFlow, &hw::OwnFlow::histogramChanged,
             _collisionAvoidanceDataProvider, &CollisionAvoidanceDataProvider::histogramReady);
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

    _converterThread.start();
    _grapherThread.start();
    _ownFlowThread.start();

    _isPaused = false;
    emit isPausedChanged(_isPaused);

    while(!_isPaused && _frameGrabber->has_next()) {
        auto currentFrame = _frameGrabber->next();
        _converter.convertImage(currentFrame);
	}

   _isPaused = true;
   emit isPausedChanged(_isPaused);
}

void OwnFlowWorker::pause() 
{	
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
}
