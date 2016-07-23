#include "RoiBuilder.h"

#include "QGCToolbox.h"
#include "MultiVehicleManager.h"
#include "Vehicle.h"

#include <cmath>        // std::abs
#include <QDebug>

using namespace std;
using namespace hw;
using namespace cv;

RoiBuilder::RoiBuilder(CollisionAvoidanceSettings& settings, QGCToolbox* toolbox, QObject* parent)
	: QObject(parent)
	, _settings(settings)
    , _toolbox(toolbox)
{
    connect(_toolbox->multiVehicleManager(), &MultiVehicleManager::activeVehicleChanged, this, &RoiBuilder::_activeVehicleChanged);
}

void RoiBuilder::_initializeRoiHelpers() 
{
    auto ocamModel = _settings.getOcamModel();
    
    auto divisor = pow(2, _settings.SubsampleAmount);
    _frameSize = cv::Size(ocamModel.width/divisor, ocamModel.height/divisor);
    _lenseCenter = cv::Point2f(ocamModel.xc/divisor, ocamModel.yc/divisor);
    _frameCenter = cv::Point2f(_frameSize.width/2, _frameSize.height/2);

    if(_settings.RawFrameRotation%2==1) // 90deg or 270deg rotation -> swap
    {
        // swap
        _frameSize = cv::Size(_frameSize.height, _frameSize.width);
        _lenseCenter = cv::Point2f(_lenseCenter.y, _lenseCenter.x);
        _frameCenter = cv::Point2f(_frameCenter.y, _frameCenter.x);
    }

    // auto p1 = _frameSize.height - _lenseCenter.y;
    // auto r1 = _frameSize.width - _lenseCenter.x;
    auto p1 = _frameSize.height - _frameCenter.y;
    auto r1 = _frameSize.width - _frameCenter.x;
    _pitchFactor = min(p1, _frameSize.height-p1) / (double)_settings.MaxPitchAngleDegrees;
    _rollFactor  = min(r1, _frameSize.width-r1) / (double)_settings.MaxRollAngleDegrees;

//    qDebug() << "Center: (" << _lenseCenter.x << ", " << _lenseCenter.y << ") size: " << _frameSize.width << "x" << _frameSize.height;
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
    // cv::Point roiCenter = _lenseCenter + offset; // plus because negative pitch indicates forwards flight
    cv::Point roiCenter = _frameCenter + offset; // plus because negative pitch indicates forwards flight

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
