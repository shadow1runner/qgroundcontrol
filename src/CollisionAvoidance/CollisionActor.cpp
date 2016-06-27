#include "CollisionActor.h"
#include "OwnFlowHandler.h"
#include "OwnFlowWorker.h"
#include "OwnFlow.h"
#include "QGCToolbox.h"
#include "MultiVehicleManager.h"
#include "Vehicle.h"
#include "QGCApplication.h"
#include "GAudioOutput.h"

CollisionActor::CollisionActor(CollisionAvoidanceSettings& settings, QGCToolbox* toolbox, QObject* parent)
    : QObject(parent)
    , _settings(settings)
    , _toolbox(toolbox)
{
    connect(_toolbox->multiVehicleManager(), &MultiVehicleManager::activeVehicleChanged, this, &CollisionActor::_activeVehicleChanged);
    connect(_toolbox->ownFlowHandler()->ownFlowWorker()->ownFlow(), &hw::OwnFlow::collisionImmanent, this, &CollisionActor::_collisionImmanent);
}

void CollisionActor::_activeVehicleChanged(Vehicle* activeVehicle)
{
    _activeVehicle = activeVehicle; // might be NULL
}

void CollisionActor::_collisionImmanent(const cv::Mat& frame, unsigned long long frameNumber, std::shared_ptr<cv::Point2i> foeFiltered, std::shared_ptr<hw::FocusOfExpansionDto> foe, const hw::CollisionLevel collisionLevel, double lastDivergence, double avgDivergence)
{
    Q_UNUSED(frame);
    Q_UNUSED(foeFiltered);
    Q_UNUSED(foe);
    Q_UNUSED(collisionLevel);
    Q_UNUSED(lastDivergence);
    Q_UNUSED(avgDivergence);

    if(_activeVehicle==nullptr)
    {
        qgcApp()->showMessage(QStringLiteral("No active vehicle chosen - unable to pause"));
        return;
    }

    if(_settings.WithholdCollisionAction) {
        auto tmp = QString("Collision in frame ") + QString::number(frameNumber) + QString(" ignored.");
        qgcApp()->showMessage(tmp);
        _toolbox->audioOutput()->say(tmp);
    } else {
        auto tmp = QString("Pausing vehicle - Collision is immanent starting from Frame ") + QString::number(frameNumber);
        _toolbox->audioOutput()->say(tmp);
        qgcApp()->showMessage(tmp);
        _activeVehicle->pauseVehicle();
    }
}
