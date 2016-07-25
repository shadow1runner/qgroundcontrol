#include "OwnFlowHandler.h"
#include "OwnFlowWorker.h"

#include <QThread>

#include "QGCToolbox.h"
#include "QtHelper.h"
#include "CollisionActor.h"
#include "OwnFlow.h"


OwnFlowHandler::OwnFlowHandler(QGCApplication* app)
    : QGCTool(app)
    , _settings(CollisionAvoidanceSettings::getInstance())
{
    QtHelper::registerMetaTypes();
    _ownFlowWorkerThread.setObjectName("OwnFlowHandler");
}

OwnFlowHandler::~OwnFlowHandler()
{
	stop();

    delete _ownFlowWorker;
}

void OwnFlowHandler::setToolbox(QGCToolbox* toolbox)
{
	QGCTool::setToolbox(toolbox);

    _ownFlowWorker = new OwnFlowWorker(_settings, toolbox);
    _ownFlowWorker->moveToThread(&_ownFlowWorkerThread);
    _ownFlowWorkerThread.start();

    _collisionActor = new CollisionActor(_settings, toolbox);

    connect(_ownFlowWorker->ownFlow(), &hw::OwnFlow::collisionImmanent,
             this, &OwnFlowHandler::_collisionImmanent);
            // Qt::BlockingQueuedConnection);
}

OwnFlowWorker* OwnFlowHandler::ownFlowWorker() 
{
    return _ownFlowWorker;
}

CollisionActor* OwnFlowHandler::collisionActor()
{
    return _collisionActor;
}

void OwnFlowHandler::start()
{
    reset();
    QThread::msleep(200);
    // invoke the method in the thread OwnFlowWorker resides in, cf. http://stackoverflow.com/a/13948634/2559632
    QMetaObject::invokeMethod(_ownFlowWorker, "start");
}

void OwnFlowHandler::pause()
{
    _ownFlowWorker->pause();
}

void OwnFlowHandler::stop()
{
    _ownFlowWorker->pause();
    _ownFlowWorkerThread.quit();
    _ownFlowWorkerThread.wait();
}

void OwnFlowHandler::reset()
{
    _ownFlowWorker->reset();
}

void OwnFlowHandler::_collisionImmanent(const cv::Mat& frame, unsigned long long frameNumber, std::shared_ptr<cv::Point2i> foeFiltered, std::shared_ptr<hw::FocusOfExpansionDto> foe, std::shared_ptr<hw::CollisionDetectorResult> detectorResult, double lastDivergence, double avgDivergence)
{
    Q_UNUSED(frame);
    Q_UNUSED(frameNumber);
    Q_UNUSED(foeFiltered);
    Q_UNUSED(foe);
    Q_UNUSED(detectorResult);
    Q_UNUSED(lastDivergence);
    Q_UNUSED(avgDivergence);

    if(_settings.WithholdCollisionAction && _settings.AutoResumeAfterCollision) {
        qDebug() << "QGC is configured to auto resume after collision -> reset+start collision avoidance";
        start();
    } else {
        qDebug() << "Pausing OwnFlowWorker because of received `collisionImmanent` event";
        QMetaObject::invokeMethod(_ownFlowWorker, "pause");
    }
}
