#include "RoiBuilder.h"

#include "QGCToolbox.h"
#include "MultiVehicleManager.h"
#include "Vehicle.h"

#include <cmath>        // std::abs
#include <QDebug>

using namespace std;
using namespace hw;
using namespace cv;

#define USE_LENSE_CENTER_INSTEAD_OF_FRAME_CENTER 1

RoiBuilder::RoiBuilder(CollisionAvoidanceSettings& settings, QGCToolbox* toolbox, QObject* parent)
	: QObject(parent)
	, _settings(settings)
    , _toolbox(toolbox)
{
    connect(_toolbox->multiVehicleManager(), &MultiVehicleManager::activeVehicleChanged, this, &RoiBuilder::_activeVehicleChanged);
}

void RoiBuilder::_initializeRoiHelpers() 
{
    if(!_settings.UndistortFrames)
        return;

    auto& ocamModel = _settings.getOcamModel();
    
    auto divisor = pow(2, _settings.SubsampleAmount);
    _frameSize = ocamModel.getFrameSize();
    _lenseCenter = ocamModel.getLenseCenter();
    _frameCenter = ocamModel.getFrameCenter();
    
#ifdef USE_LENSE_CENTER_INSTEAD_OF_FRAME_CENTER
    auto p1 = _frameSize.height - _lenseCenter.y;
    auto r1 = _frameSize.width - _lenseCenter.x;
#elif
    auto p1 = _frameSize.height - _frameCenter.y;
    auto r1 = _frameSize.width - _frameCenter.x;
#endif
    _pitchFactor = min(p1, _frameSize.height-p1) / (double)_settings.MaxPitchAngleDegrees;
    _rollFactor  = min(r1, _frameSize.width-r1) / (double)_settings.MaxRollAngleDegrees;
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
    if((_altitudeRelative>altitudeRelative && climbRate>0.0) ||
       (_altitudeRelative<altitudeRelative && climbRate<0.0))
        _climbRate = -climbRate;
    else
        _climbRate = climbRate;

    _altitudeRelative = altitudeRelative;
}

void RoiBuilder::_attitudeChanged(UASInterface*, double roll, double pitch, double yaw, quint64)
{
    Q_UNUSED(yaw);
    roll = _toDegrees(roll);
    pitch = _toDegrees(pitch);
    _calculateRoi(roll, pitch);
}

double RoiBuilder::_toDegrees(double rad)
{
    if (qIsInf(rad)) {
        rad = 0;
    } else {
        rad = rad * (180.0 / M_PI);
    }
    return rad;
}

void RoiBuilder::_calculateRoi(double rollDegree, double pitchDegree)
{
    if(!_roiHelpersInitialized) {
        _initializeRoiHelpers();
        _roiHelpersInitialized = true;
    }

    // given: _climbRate, roll, pitch; imageSize and omni camera center
    // target: calculate image roi where a FOE (after KF) for a possible obstacle __should__ be

    // determine new center of roi, dependent on roll and pitch values
    cv::Point2f offset(rollDegree*this->_rollFactor, pitchDegree*_pitchFactor);
#ifdef USE_LENSE_CENTER_INSTEAD_OF_FRAME_CENTER
    cv::Point roiCenter = _lenseCenter + offset; // plus because negative pitch indicates forwards flight
#elif
    cv::Point roiCenter = _frameCenter + offset; // plus because negative pitch indicates forwards flight
#endif

    // determine ROI as a rect
    cv::Rect roi(roiCenter.x-_settings.RoiWidthPx/2, roiCenter.y-_settings.RoiHeightPx/2, _settings.RoiWidthPx, _settings.RoiHeightPx);

    // clamp
    if(roi.x < 0) {
        roi.width -= abs(roi.x);
        roi.x = 0;
    }
    if(roi.y < 0) {
        roi.height -= abs(roi.y);
        roi.y = 0;
    }
    if(roi.x + roi.width > _frameSize.width)
        roi.width = _frameSize.width-roi.x;
    if(roi.y + roi.height > _frameSize.height)
        roi.height = _frameSize.height-roi.y;

    emit roiReady(roi, rollDegree, pitchDegree, _climbRate);
}
