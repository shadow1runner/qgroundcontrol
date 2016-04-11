#include "OwnFlowWorker.h"
#include "OwnFlowHandler.h"

#include "QGCToolbox.h"
#include "UAS.h"
#include "MultiVehicleManager.h"
#include "QGC.h"

OwnFlowWorker::OwnFlowWorker(const CollisionAvoidanceSettings& settings, QGCToolbox* toolbox)
    : QObject(NULL)
	, _settings(settings)
    , _collisionAvoidanceDataProvider(toolbox->collisionAvoidanceDataProvider())
    , _isPaused(false)
    , _converter(settings.getSubsampleAmount())
    , _ownFlow(settings.getParticles(), settings.getWindowSize())
{ 
    _converterThread.setObjectName("OwnFlowConverter");
    _ownFlowThread.setObjectName("OwnFlow");

    _converter.moveToThread(&_converterThread);

    _ownFlow.moveToThread(&_ownFlowThread);

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

    connect(&_ownFlow, &hw::OwnFlow::opticalFlowChanged,
             _collisionAvoidanceDataProvider, &CollisionAvoidanceDataProvider::opticalFlowReady);
    
    connect(&_ownFlow, &hw::OwnFlow::histogramChanged,
             _collisionAvoidanceDataProvider, &CollisionAvoidanceDataProvider::histogramReady);

    // connect to signals emitted by `OwnFlow` - used for emitting valueChanged signals (and thus for graphing)
    connect(&_ownFlow, &hw::OwnFlow::foeChanged,
            this, &OwnFlowWorker::_handleCollisionAvoidance
            );

    connect(&_ownFlow, &hw::OwnFlow::collisionAvoidanceFrameTimingsReady,
            this, &OwnFlowWorker::_handleCollisionAvoidanceFrameTimings
            );

    connect(toolbox->multiVehicleManager(), &MultiVehicleManager::activeVehicleChanged,
            this, &OwnFlowWorker::_activeVehicleChanged
            );
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
} 

void OwnFlowWorker::_handleCollisionAvoidance(
    const cv::Mat& frame,
    std::shared_ptr<hw::FocusOfExpansionDto> foeFiltered,
    std::shared_ptr<hw::FocusOfExpansionDto> foeMeasured,
    std::shared_ptr<hw::Divergence> divergence)
{
    Q_UNUSED(frame);
    
    emit valueChanged(_activeUas->getUASID(),"foeEkfx","px",QVariant(foeFiltered->getFoE().x), getUnixTime());
    emit valueChanged(_activeUas->getUASID(),"foeEkfy","px",QVariant(foeFiltered->getFoE().y), getUnixTime());
    emit valueChanged(_activeUas->getUASID(),"foeRawx","px",QVariant(foeMeasured->getFoE().x), getUnixTime());
    emit valueChanged(_activeUas->getUASID(),"foeRawy","px",QVariant(foeMeasured->getFoE().y), getUnixTime());
    emit valueChanged(_activeUas->getUASID(),"divergence","-",QVariant(divergence->getDivergence()), getUnixTime());
    emit valueChanged(_activeUas->getUASID(),"inlierRatio","‰",QVariant(foeFiltered->getInlierProportion()*1000), getUnixTime());
}

void OwnFlowWorker::_handleCollisionAvoidanceFrameTimings(
    std::shared_ptr<AvgWatch> allWatch,
    std::shared_ptr<AvgWatch> colliderWatch,
    std::shared_ptr<AvgWatch> divWatch,
    std::shared_ptr<AvgWatch> foeWatch,
    std::shared_ptr<AvgWatch> kalmanWatch,
    std::shared_ptr<AvgWatch> opticalFlowWatch)
{
    emit valueChanged(_activeUas->getUASID(), "collidingTime", "s", QVariant(colliderWatch->elapsedLast()/1e9), getUnixTime());
    emit valueChanged(_activeUas->getUASID(), "divergenceTime", "s", QVariant(divWatch->elapsedLast()/1e9), getUnixTime());
    emit valueChanged(_activeUas->getUASID(), "foeTime", "s", QVariant(foeWatch->elapsedLast()/1e9), getUnixTime());
    emit valueChanged(_activeUas->getUASID(), "kalmanTime", "s", QVariant(kalmanWatch->elapsedLast()/1e9), getUnixTime());
    emit valueChanged(_activeUas->getUASID(), "opticalFlowTime", "s", QVariant(opticalFlowWatch->elapsedLast()/1e9), getUnixTime());

    auto sec = allWatch->elapsedLast()/1e9;
    auto fps = 1.0/sec;
    emit valueChanged(_activeUas->getUASID(),"fps","-",QVariant(fps), getUnixTime());
}

void OwnFlowWorker::_activeVehicleChanged(Vehicle* activeVehicle)
{
  _activeUas = activeVehicle->uas(); // might be NULL
}

// note: the actual (=correct) implementation isfound in UAS::getUnixTime
quint64 OwnFlowWorker::getUnixTime()
{
    return QGC::groundTimeMilliseconds();
}
