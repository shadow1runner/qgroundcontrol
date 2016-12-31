#ifndef COLLISIONAVOIDANCE_ROIBUILDER_H
#define COLLISIONAVOIDANCE_ROIBUILDER_H

#include "CollisionAvoidanceSettings.h"
#include "UASInterface.h"

#include <opencv2/core.hpp>

#include <QObject>

class QGCToolbox;
class Vehicle;
class UAS;

class RoiBuilder : public QObject
{
    Q_OBJECT

    CollisionAvoidanceSettings& _settings;
    QGCToolbox * const _toolbox;
    
    Vehicle* _activeVehicle = nullptr;
    double _climbRate = 0.0;
    double _altitudeRelative = 0.0;

    // dependent on settings.getOcamModel; determine image center and frame size, needed for ROI calculation
    cv::Size _frameSize;
    cv::Point2f _lenseCenter;
    cv::Point2f _frameCenter;

    double _pitchFactor;
    double _rollFactor;
    bool _roiHelpersInitialized = false;
    
private:
    void _initializeRoiHelpers();
    double _toDegrees(double rad);
    void _calculateRoi(double rollDegree, double pitchDegree);

private slots:
    void _activeVehicleChanged(Vehicle* activeVehicle);
    void _altitudeChanged(UASInterface* uas, double _altitudeAMSL, double _altitudeRelative, double _climbRate, quint64 timestamp);
    void _attitudeChanged(UASInterface* uas, double roll, double pitch, double yaw, quint64 timestamp);

signals:
    void roiReady(const cv::Rect& roi, double rollDegree, double pitchDegree, double climbRate);
    
public:
    RoiBuilder(CollisionAvoidanceSettings& settings, QGCToolbox* toolbox, QObject* parent=NULL);
};

#endif // COLLISIONAVOIDANCE_ROIBUILDER_H
