#include "CollisionActor.h"
#include "OwnFlowHandler.h"
#include "OwnFlowWorker.h"
#include "OwnFlow.h"
#include "QGCToolbox.h"
#include "MultiVehicleManager.h"
#include "Vehicle.h"
#include "QGCApplication.h"

CollisionActor::CollisionActor(CollisionAvoidanceSettings& settings, QGCToolbox* toolbox, QObject* parent)
    : QObject(parent)
    , _settings(settings)
{
    connect(toolbox->multiVehicleManager(), &MultiVehicleManager::activeVehicleChanged, this, &CollisionActor::_activeVehicleChanged);
    connect(toolbox->ownFlowHandler()->ownFlowWorker()->ownFlow(), &hw::OwnFlow::collisionImmanent, this, &CollisionActor::_collisionImmanent);
}

void CollisionActor::_activeVehicleChanged(Vehicle* activeVehicle)
{
    _activeVehicle = activeVehicle; // might be NULL
}

void CollisionActor::_collisionImmanent(const cv::Mat& frame, std::shared_ptr<cv::Point2i> foeFiltered, std::shared_ptr<hw::FocusOfExpansionDto> foe, const hw::CollisionLevel collisionLevel)
{
    Q_UNUSED(frame);
    Q_UNUSED(foeFiltered);
    Q_UNUSED(foe);
    Q_UNUSED(collisionLevel);

    if(_activeVehicle==nullptr)
    {
        qgcApp()->showMessage(QStringLiteral("No active vehicle chosen - unable to pause"));
        return;
    }

    qgcApp()->showMessage(QStringLiteral("Pausing vehicle - Collision is immanent"));
    _activeVehicle->pauseVehicle();
}
