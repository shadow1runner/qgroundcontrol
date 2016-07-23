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
    
    UAS* _activeUas = nullptr;
    double _climbRate = 0.0;
    double _altitudeRelative = 0.0;
    double _roll = 0.0;
    double _pitch = 0.0;

    // dependent on settings.getOcamModel; determine image center and frame size, needed for ROI calculation
    cv::Size _size;
    cv::Point _center;

    void calculateRoi();

private slots:
    void _activeVehicleChanged(Vehicle* activeVehicle);
    void _altitudeChanged(UASInterface* uas, double _altitudeAMSL, double _altitudeRelative, double _climbRate, quint64 timestamp);
    void _attitudeChanged(UASInterface* uas, double roll, double pitch, double yaw, quint64 timestamp);
    
public:
    RoiBuilder(CollisionAvoidanceSettings& settings, QGCToolbox* toolbox, QObject* parent=NULL);


};

#endif // COLLISIONAVOIDANCE_ROIBUILDER_H
