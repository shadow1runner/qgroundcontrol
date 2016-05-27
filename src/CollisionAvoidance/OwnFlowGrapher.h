#ifndef OWNFLOWGRAPHER_H
#define OWNFLOWGRAPHER_H

#include <QObject>

#include "OwnFlow.h"
#include "CollisionLevel.h"

class UAS;
class Vehicle;
class QGCToolbox;

class OwnFlowGrapher : public QObject
{
  Q_OBJECT
public:
  explicit OwnFlowGrapher(hw::OwnFlow* const ownFlow, QGCToolbox* toolbox, QObject *parent = 0);

  signals:    
	/** @brief A value of the robot has changed.
      *
      * Typically this is used to send lowlevel information like the battery voltage to the plotting facilities of
      * the groundstation. The data here should be converted to human-readable values before being passed, so ideally
      * SI units.
      *
      * @param uasId ID of this system
      * @param name name of the value, e.g. "battery voltage"
      * @param unit The units this variable is in as an abbreviation. For system-dependent (such as raw ADC values) use "raw", for bitfields use "bits", for true/false or on/off use "bool", for unitless values use "-".
      * @param value the value that changed
      * @param msec the timestamp of the message, in milliseconds
      */
      void valueChanged(const int uasid, const QString& name, const QString& unit, const QVariant &value,const quint64 msecs);

      private slots:
      void _handleCollisionAvoidance(const cv::Mat& frame, unsigned long long frameNumber, std::shared_ptr<cv::Point2i> foeFiltered, std::shared_ptr<hw::FocusOfExpansionDto> foe, const hw::CollisionLevel collisionLevel, double lastDivergence, double avgDivergence);
      void _handleCollisionAvoidanceBadFrame(const cv::Mat& badFrame, unsigned long long skipFrameCount, unsigned long long totalFrameCount, std::shared_ptr<hw::FocusOfExpansionDto> foeMeasured);
      void _handleCollisionAvoidanceFrameTimings(std::shared_ptr<AvgWatch> allWatch, std::shared_ptr<AvgWatch> colliderWatch, std::shared_ptr<AvgWatch> divWatch, std::shared_ptr<AvgWatch> foeWatch, std::shared_ptr<AvgWatch> kalmanWatch, std::shared_ptr<AvgWatch> opticalFlowWatch);
      void _activeVehicleChanged(Vehicle* activeVehicle);

    private:
      hw::OwnFlow* const _ownFlow;
      // note: the actual (=correct) implementation isfound in UAS::getUnixTime
      quint64 getUnixTime();
      int getUASID();
      UAS* _activeUas = nullptr;
    };

#endif // OWNFLOWGRAPHER_H
