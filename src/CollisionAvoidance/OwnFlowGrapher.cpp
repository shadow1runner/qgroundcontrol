#include "OwnFlowGrapher.h"

#include <string>
#include <memory>
#include <opencv2/core.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include <QObject>
#include <QThread>
#include <QDateTime>

#include "QGCToolbox.h"
#include "UAS.h"
#include "MultiVehicleManager.h"
#include "QGC.h"
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

OwnFlowGrapher::OwnFlowGrapher(hw::OwnFlow* const ownFlow, QGCToolbox* toolbox, QObject *parent)
	: QObject(parent)
	, _ownFlow(ownFlow)
{
	// connect to signals emitted by `OwnFlow` - used for emitting valueChanged signals (and thus for graphing)
    connect(_ownFlow, &hw::OwnFlow::collisionLevelRatingReady,
            this, &OwnFlowGrapher::_handleCollisionAvoidance
            );

    connect(_ownFlow, &hw::OwnFlow::frameSkipped,
	        this, &OwnFlowGrapher::_handleCollisionAvoidanceBadFrame
	        );

    connect(_ownFlow, &hw::OwnFlow::collisionAvoidanceFrameTimingsReady,
	        this, &OwnFlowGrapher::_handleCollisionAvoidanceFrameTimings
	        );

	connect(toolbox->multiVehicleManager(), &MultiVehicleManager::activeVehicleChanged,
	        this, &OwnFlowGrapher::_activeVehicleChanged
	        );
}

void OwnFlowGrapher::_handleCollisionAvoidance(const cv::Mat& frame, unsigned long long frameNumber, std::shared_ptr<cv::Point2i> foeFiltered, std::shared_ptr<hw::FocusOfExpansionDto> foe, const hw::CollisionLevel collisionLevel, double lastDivergence, double avgDivergence)
{
    Q_UNUSED(frame);
    Q_UNUSED(frameNumber);
    
    emit valueChanged(getUASID(),"foeEkfx","px",QVariant(foeFiltered->x), getUnixTime());
    emit valueChanged(getUASID(),"foeEkfy","px",QVariant(foeFiltered->y), getUnixTime());
    emit valueChanged(getUASID(),"foeRawx","px",QVariant(foe->getFoE().x), getUnixTime());
    emit valueChanged(getUASID(),"foeRawy","px",QVariant(foe->getFoE().y), getUnixTime());
    emit valueChanged(getUASID(),"collisionLevel","-",QVariant(collisionLevel), getUnixTime());
    emit valueChanged(getUASID(),"divergence","-",QVariant(lastDivergence), getUnixTime());
    emit valueChanged(getUASID(),"avgDivergence","-",QVariant(avgDivergence), getUnixTime());
    emit valueChanged(getUASID(),"inlierRatio","‰",QVariant(foe->getInlierProportion()*1000), getUnixTime());
}

void OwnFlowGrapher::_handleCollisionAvoidanceBadFrame(
    const cv::Mat& badFrame, 
    unsigned long long skipFrameCount, 
    unsigned long long totalFrameCount,
    std::shared_ptr<hw::FocusOfExpansionDto> foeMeasured)
{
    Q_UNUSED(badFrame);
    Q_UNUSED(foeMeasured);

    emit valueChanged(getUASID(), "skipFrameRatio", "-", QVariant(skipFrameCount/(double)totalFrameCount), getUnixTime());
    emit valueChanged(getUASID(), "skipFrames", "-", QVariant(skipFrameCount), getUnixTime());
}

void OwnFlowGrapher::_handleCollisionAvoidanceFrameTimings(
    std::shared_ptr<AvgWatch> allWatch,
    std::shared_ptr<AvgWatch> colliderWatch,
    std::shared_ptr<AvgWatch> divWatch,
    std::shared_ptr<AvgWatch> foeWatch,
    std::shared_ptr<AvgWatch> kalmanWatch,
    std::shared_ptr<AvgWatch> opticalFlowWatch)
{
    emit valueChanged(getUASID(), "collidingTime", "s", QVariant(colliderWatch->elapsedLast()/1e9), getUnixTime());
    emit valueChanged(getUASID(), "divergenceTime", "s", QVariant(divWatch->elapsedLast()/1e9), getUnixTime());
    emit valueChanged(getUASID(), "foeTime", "s", QVariant(foeWatch->elapsedLast()/1e9), getUnixTime());
    emit valueChanged(getUASID(), "kalmanTime", "s", QVariant(kalmanWatch->elapsedLast()/1e9), getUnixTime());
    emit valueChanged(getUASID(), "opticalFlowTime", "s", QVariant(opticalFlowWatch->elapsedLast()/1e9), getUnixTime());

    auto sec = allWatch->elapsedLast()/1e9;
    auto fps = 1.0/sec;
    emit valueChanged(getUASID(),"fps","-",QVariant(fps), getUnixTime());
}

void OwnFlowGrapher::_activeVehicleChanged(Vehicle* activeVehicle)
{
	if(activeVehicle!=NULL)
		_activeUas = activeVehicle->uas(); // might be NULL
	else
		_activeUas = nullptr;
}

// note: the actual (=correct) implementation isfound in UAS::getUnixTime
quint64 OwnFlowGrapher::getUnixTime()
{
    return QGC::groundTimeMilliseconds();
}

int OwnFlowGrapher::getUASID()
{
	return _activeUas!=nullptr ? _activeUas->getUASID() : -1;
}


