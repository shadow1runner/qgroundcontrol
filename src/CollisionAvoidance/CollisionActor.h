#ifndef COLLISIONACTOR_H
#define COLLISIONACTOR_H

#include <QObject>

#include <opencv2/core.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include "DrawHelper.h"
#include "HeatMap.h"
#include "OwnFlow.h"
#include "FocusOfExpansionDto.h"
#include "CollisionAvoidanceSettings.h"

class QGCToolbox;
class Vehicle;

class CollisionActor : public QObject
{
    Q_OBJECT

public:
    CollisionActor(CollisionAvoidanceSettings& settings, QGCToolbox* toolbox, QObject* parent=0);

private slots:
    void _activeVehicleChanged(Vehicle* activeVehicle);
    void _collisionImmanent(const cv::Mat& frame, unsigned long long frameNumber, std::shared_ptr<cv::Point2i> foeFiltered, std::shared_ptr<hw::FocusOfExpansionDto> foe, const hw::CollisionLevel collisionLevel);

private:
	CollisionAvoidanceSettings& _settings;
	Vehicle* _activeVehicle = nullptr;
};

#endif // COLLISIONACTOR_H
