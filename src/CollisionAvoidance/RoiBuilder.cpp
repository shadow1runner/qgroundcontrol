#include "RoiBuilder.h"

#include "QGCToolbox.h"
#include "MultiVehicleManager.h"
#include "Vehicle.h"

#include <cmath>        // std::abs
#include <QDebug>

RoiBuilder::RoiBuilder(CollisionAvoidanceSettings& settings, QGCToolbox* toolbox, QObject* parent)
	: QObject(parent)
	, _settings(settings)
    , _toolbox(toolbox)
{
    connect(_toolbox->multiVehicleManager(), &MultiVehicleManager::activeVehicleChanged, this, &RoiBuilder::_activeVehicleChanged);

    auto ocamModel = _settings.getOcamModel();
    
    auto divisor = pow(2, _settings.SubsampleAmount);
    _size = cv::Size(ocamModel.width/divisor, ocamModel.height/divisor);
    _center = cv::Point(ocamModel.xc/divisor, ocamModel.yc/divisor);

    if(_settings.RawFrameRotation%2==1) // 90deg or 270deg rotation -> swap
    {
        // swap
        _size = cv::Size(_size.height, _size.width);
        _center = cv::Point(_center.y, _center.x);
    }

    qDebug() << "Center: (" << _center.x << ", " << _center.y << ") size: " << _size.width << "x" << _size.height;
}

void RoiBuilder::_activeVehicleChanged(Vehicle* activeVehicle)
{
    if(activeVehicle!=nullptr)
    {
        _activeUas = activeVehicle->uas(); // might be NULL
        if(_activeUas!=nullptr) { 
            connect((UASInterface*)_activeUas, &UASInterface::altitudeChanged, this, &RoiBuilder::_altitudeChanged);
            connect((UASInterface*)_activeUas, SIGNAL(attitudeChanged(UASInterface*,double,double,double,quint64)), this, SLOT(_attitudeChanged(UASInterface*, double, double, double, quint64)));
        }
    }
}

void RoiBuilder::_altitudeChanged(UASInterface*, double altitudeAMSL, double altitudeRelative, double climbRate, quint64)
{
    Q_UNUSED(altitudeAMSL);

    // for unknown reasons the climbrate values seem to be alternating, even though the MAV is steadily climbing/descending -> correct it heurisitcally
    // fun fact though: the `Analyze` widget of QGC does not show these alternating values, even though the very same values are published
    if(_altitudeRelative>altitudeRelative && climbRate>0.0)
        _climbRate = -climbRate;
    else
        _climbRate = climbRate;

    _altitudeRelative = altitudeRelative;
}

void RoiBuilder::_attitudeChanged(UASInterface*, double roll, double pitch, double yaw, quint64)
{
    Q_UNUSED(yaw);

    if (qIsInf(roll)) {
        _roll = 0;
    } else {
        _roll = roll * (180.0 / M_PI);
    }
    if (qIsInf(pitch)) {
        _pitch = 0;
    } else {
        _pitch = pitch * (180.0 / M_PI);
    }

    calculateRoi();
}

void RoiBuilder::calculateRoi() 
{
    // given: _climbRate, _roll, _pitch, _yaw; imageSize and omni camera center
    // target: calculate image roi where a FOE (after KF) for a possible obstacle __should__ be
}
