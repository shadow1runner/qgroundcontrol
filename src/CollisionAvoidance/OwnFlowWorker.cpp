#include "OwnFlowWorker.h"
#include "OwnFlowHandler.h"

OwnFlowWorker::OwnFlowWorker(const CollisionAvoidanceSettings& settings, const CollisionAvoidanceDataProvider* const collisionAvoidanceDataProvider)
    : QObject(NULL)
	, _settings(settings)
    , _collisionAvoidanceDataProvider(collisionAvoidanceDataProvider)
    , _isPaused(false)
    , _converter(settings.getSubsampleAmount())
    , _ownFlow(settings.getParticles(), settings.getWindowSize())
{ 
    _converter.moveToThread(&_converterThread);

    _ownFlow.moveToThread(&_ownFlowThread);

    if(settings.getUseRecordedVideoInsteadOfDevice()) 
        _frameGrabber = new hw::BufferedFrameGrabber(settings.getFileName().toStdString(), 1, [](cv::Mat input) {return input;});
    else
        _frameGrabber = new hw::BufferedFrameGrabber(settings.getDevice(), 1, [](cv::Mat input) {return input;});

    connect(&_converter, &hw::Converter::imageConverted,
            &_ownFlow, &hw::OwnFlow::processImage
            //,Qt::BlockingQueuedConnection);
           );

    connect(&_ownFlow, &hw::OwnFlow::foeChanged,
             _collisionAvoidanceDataProvider, &CollisionAvoidanceDataProvider::foeReady
           );//Qt::BlockingQueuedConnection);

    connect(&_ownFlow, &hw::OwnFlow::opticalFlowChanged,
             _collisionAvoidanceDataProvider, &CollisionAvoidanceDataProvider::opticalFlowReady);
    
    connect(&_ownFlow, &hw::OwnFlow::histogramChanged,
             _collisionAvoidanceDataProvider, &CollisionAvoidanceDataProvider::histogramReady);

	// initialize baseFrame
    _converter.convertImage(_frameGrabber->next());
}

OwnFlowWorker::~OwnFlowWorker() {
    delete _frameGrabber;
}

hw::OwnFlow* OwnFlowWorker::ownFlow() {
    return &_ownFlow;
}

void OwnFlowWorker::start() 
{
    qDebug() << "Starting OwnFlowWorker on Thread" << QThread::currentThreadId();

    _converterThread.start();
    _ownFlowThread.start();

    _isPaused = false;
    emit isPausedChanged(_isPaused);

    while(!_isPaused && _frameGrabber->has_next()) {
        auto currentFrame = _frameGrabber->next();
        _converter.convertImage(currentFrame);
	}

//    _isPaused = true;
//    emit isPausedChanged(_isPaused);
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
} 
