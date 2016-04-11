#ifndef OWNFLOWWORKER_H
#define OWNFLOWWORKER_H

#include <string>
#include <memory>
#include <opencv2/core.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include <QObject>
#include <QThread>

#include "QGCToolbox.h"
#include "CollisionAvoidanceSettings.h"
#include "CollisionAvoidanceDataProvider.h"
#include "Converter.h"
#include "OwnFlow.h"
#include "BufferedFrameGrabber.h"
#include "FocusOfExpansionDto.h"
#include "CollisionAvoidanceDataProvider.h"
#include "FocusOfExpansionDto.h"
#include "Divergence.h"
#include "AvgWatch.h"

class OwnFlowHandler;
class QGCToolbox;
class UAS;

class OwnFlowWorker : public QObject {
	Q_OBJECT

public:
    OwnFlowWorker(const CollisionAvoidanceSettings& settings, QGCToolbox* toolbox);
    ~OwnFlowWorker();

    hw::OwnFlow* ownFlow();

public slots:
	void start();
    void pause();
	void stop();

signals:
    void isPausedChanged(bool isPaused);
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
    void _handleCollisionAvoidance(const cv::Mat& frame, std::shared_ptr<hw::FocusOfExpansionDto> foeFiltered, std::shared_ptr<hw::FocusOfExpansionDto> foeMeasured, std::shared_ptr<hw::Divergence> divergence);
    void _handleCollisionAvoidanceFrameTimings(std::shared_ptr<AvgWatch> allWatch, std::shared_ptr<AvgWatch> colliderWatch, std::shared_ptr<AvgWatch> divWatch, std::shared_ptr<AvgWatch> foeWatch, std::shared_ptr<AvgWatch> kalmanWatch, std::shared_ptr<AvgWatch> opticalFlowWatch);
    void _activeVehicleChanged  (Vehicle* activeVehicle);

private:
    // note: the actual (=correct) implementation isfound in UAS::getUnixTime
    quint64 getUnixTime();

private:
    const CollisionAvoidanceSettings& _settings;
    const CollisionAvoidanceDataProvider * const _collisionAvoidanceDataProvider = nullptr;
    UAS* _activeUas;

    bool _isPaused;

    hw::BufferedFrameGrabber* _frameGrabber = nullptr;

	  hw::Converter _converter;
    hw::OwnFlow _ownFlow;

    QThread _ownFlowThread;
    QThread _converterThread;
};

#endif // OWNFLOWWORKER_H
