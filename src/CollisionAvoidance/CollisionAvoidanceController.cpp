#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/imgproc.hpp>
//#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <string>
#include <unistd.h>
#include <chrono>
#include <thread>

#include "CollisionAvoidanceController.h"
#include "UAS.h"
#include "QGCApplication.h"
#include "BufferedFrameGrabber.h"
#include "config.h"
#include "OwnFlow.h"
#include "Converter.h"
#include "FocusOfExpansionDto.h"
#include "StopWatch.h"
#include "AvgWatch.h"
#include "Macros.h"
#include "Displayer.h"
#include "ConsoleDisplayer.h"
#include "QtHelper.h"

#include <QSettings>
#include <QUrl>
#include <QDebug>

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

void CollisionAvoidanceController::setHasCollisionAvoidanceStream(bool newValue)
{
	if(_hasCollisionAvoidanceStream!=newValue) {
		_hasCollisionAvoidanceStream = newValue;
		emit hasCollisionAvoidanceStreamChanged(newValue);
	}
}

void CollisionAvoidanceController::onStartStopClicked(bool started)
{
    qDebug() << started;
}

void CollisionAvoidanceController::_activeVehicleChanged(Vehicle* activeVehicle)
{
    if(activeVehicle)
        _uas = activeVehicle->uas();
}
