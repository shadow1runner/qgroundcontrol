#include "CollisionAvoidanceController.h"
#include "UAS.h"
#include "QGCApplication.h"

#include <QSettings>
#include <QUrl>

CollisionAvoidanceController::CollisionAvoidanceController(void) :
    _uas(NULL)
{
    if(qgcApp()->toolbox()->multiVehicleManager()->activeVehicle()) {
        _uas = qgcApp()->toolbox()->multiVehicleManager()->activeVehicle()->uas();
    }
//    QSettings settings;
//    _customQmlFile = settings.value(_settingsKey).toString();
    connect(qgcApp()->toolbox()->multiVehicleManager(), &MultiVehicleManager::activeVehicleChanged, this, &CollisionAvoidanceController::_activeVehicleChanged);
}

void CollisionAvoidanceController::_activeVehicleChanged(Vehicle* activeVehicle)
{
    if(activeVehicle)
        _uas = activeVehicle->uas();
}
