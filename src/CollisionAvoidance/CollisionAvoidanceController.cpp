#include "CollisionAvoidanceController.h"
#include "UAS.h"
#include "QGCApplication.h"

#include <QSettings>
#include <QUrl>

CollisionAvoidanceController::CollisionAvoidanceController(QObject* parent)
    : QObject(parent)
    , _uas(NULL)
    , _hasCollisionAvoidanceStream(true)
{
    if(qgcApp()->toolbox()->multiVehicleManager()->activeVehicle()) {
        _uas = qgcApp()->toolbox()->multiVehicleManager()->activeVehicle()->uas();
    }
//    QSettings settings;
//    _customQmlFile = settings.value(_settingsKey).toString();
    connect(qgcApp()->toolbox()->multiVehicleManager(), &MultiVehicleManager::activeVehicleChanged, this, &CollisionAvoidanceController::_activeVehicleChanged);
}

void CollisionAvoidanceController::setHasCollisionAvoidanceStream (bool newValue) {
	if(_hasCollisionAvoidanceStream!=newValue) {
		_hasCollisionAvoidanceStream = newValue;
		emit hasCollisionAvoidanceStreamChanged(newValue);
	}
}


void CollisionAvoidanceController::_activeVehicleChanged(Vehicle* activeVehicle)
{
    if(activeVehicle)
        _uas = activeVehicle->uas();
}
