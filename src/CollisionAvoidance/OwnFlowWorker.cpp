#include "OwnFlowWorker.h"
#include "OwnFlowHandler.h"

OwnFlowWorker::OwnFlowWorker(const CollisionAvoidanceSettings& settings, const CollisionAvoidanceDataProvider* const collisionAvoidanceDataProvider)
    : QObject(NULL)
	, _settings(settings)
    , _collisionAvoidanceDataProvider(collisionAvoidanceDataProvider)
    , _converter(settings.getSubsampleAmount())
    , _ownFlow(settings.getParticles(), settings.getWindowSize())
{ 
    qDebug() << QString::fromStdString(_settings.getFileName());
    _converter.moveToThread(&_converterThread);

    _ownFlow.moveToThread(&_ownFlowThread);

    connect(&_converter, &hw::Converter::imageConverted,
            &_ownFlow, &hw::OwnFlow::processImage
           );//Qt::BlockingQueuedConnection);

    connect(&_ownFlow, &hw::OwnFlow::foeChanged,
             _collisionAvoidanceDataProvider, &CollisionAvoidanceDataProvider::foeReady
           );//Qt::BlockingQueuedConnection);

    connect(&_ownFlow, &hw::OwnFlow::opticalFlowChanged,
             _collisionAvoidanceDataProvider, &CollisionAvoidanceDataProvider::opticalFlowReady);
    
    connect(&_ownFlow, &hw::OwnFlow::histogramChanged,
             _collisionAvoidanceDataProvider, &CollisionAvoidanceDataProvider::histogramReady);
}

OwnFlowWorker::~OwnFlowWorker() {
}

hw::OwnFlow* OwnFlowWorker::ownFlow() {
    return &_ownFlow;
}

void OwnFlowWorker::start() 
{
    qDebug() << "Starting OwnFlowWorker on Thread" << QThread::currentThreadId();

    _converterThread.start();
    _ownFlowThread.start();

    qDebug() << QString::fromStdString(_settings.getFileName());
    hw::BufferedFrameGrabber frame_grabber(_settings.getFileName(), 1, [](cv::Mat input) {return input;});

	// initialize baseFrame
    _converter.convertImage(frame_grabber.next());

    while(frame_grabber.has_next()) {
        auto currentFrame = frame_grabber.next();
        _converter.convertImage(currentFrame);
	}
}

void OwnFlowWorker::stop() 
{	
    _converterThread.quit();
    _converterThread.wait();

    _ownFlowThread.quit();
    _ownFlowThread.wait();
} 
