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

using namespace std;
using namespace hw;

OwnFlowGrapher::OwnFlowGrapher(hw::OwnFlow* const ownFlow, QGCToolbox* toolbox, const CollisionAvoidanceSettings& settings, QObject *parent)
	: QObject(parent)
	, _ownFlow(ownFlow)
    , _settings(settings)
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

    // log file
    csvFile.open(_settings.CsvFilePath.toStdString(), ofstream::out | ofstream::trunc);
    if(!csvFile.is_open())
    {
        throw new std::invalid_argument(_settings.CsvFilePath.toStdString());
    }

    writeCsvHeader();
}

OwnFlowGrapher::~OwnFlowGrapher()
{
    csvFile.close();
}


void OwnFlowGrapher::_handleCollisionAvoidance(const cv::Mat& frame, unsigned long long frameNumber, std::shared_ptr<cv::Point2i> foeFiltered, std::shared_ptr<hw::FocusOfExpansionDto> foe, std::shared_ptr<hw::CollisionDetectorResult> detectorResult, double lastDivergence, double avgDivergence)
{
    Q_UNUSED(frame);
    Q_UNUSED(frameNumber);

    logGoodFrameToCsv(frameNumber, foeFiltered, foe, detectorResult, lastDivergence, avgDivergence);
    
    emit valueChanged(getUASID(),"foeEkfx","px",QVariant(foeFiltered->x), getUnixTime());
    emit valueChanged(getUASID(),"foeEkfy","px",QVariant(foeFiltered->y), getUnixTime());
    emit valueChanged(getUASID(),"foeRawx","px",QVariant(foe->getFoE().x), getUnixTime());
    emit valueChanged(getUASID(),"foeRawy","px",QVariant(foe->getFoE().y), getUnixTime());
    emit valueChanged(getUASID(),"collisionLevel","-",QVariant(detectorResult->getCollisionLevel()), getUnixTime());
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

    logBadFrameToCsv(totalFrameCount, foeMeasured);

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

void OwnFlowGrapher::writeCsvHeader()
{
    csvFile << "#";
    csvFile << ",CollisionLevel";
    csvFile << ",CollisionLevel (string)";
    csvFile << ",Detector Evaluation Result";
    csvFile << ",Divergence (last)";
    csvFile << ",Divergence (average over " << _settings.DivergenceHistoryBufferSize << " elements)";
    csvFile << ",EKF FoE x";
    csvFile << ",EKF FoE y";
    csvFile << ",Inlier Proportion"; if(_settings.InlierProportionThresholdEnabled) csvFile << " (bad if < " << _settings.InlierProportionThreshold << ")";
    csvFile << ",FoE x";
    csvFile << ",FoE y";
    csvFile << ",Number of Collision Inliers";
    csvFile << ",Number of Collision Particles";
    csvFile << std::endl;
}

void OwnFlowGrapher::logBadFrameToCsv(unsigned long long frameNumber, std::shared_ptr<hw::FocusOfExpansionDto> foeMeasured)
{
    csvFile << frameNumber;
    csvFile << ","; // << static_cast<int>(CollisionLevel);
    csvFile << ","; // << CollisionHelper::toString(CollisionLevel);
    csvFile << ","; // << detectorResult->getEvaluationResultText();
    csvFile << ","; // << lastDivergence;
    csvFile << ","; // << avgDivergence;
    csvFile << ","; // << foeFiltered->x;
    csvFile << ","; // << foeFiltered->y;
    csvFile << "," << foeMeasured->getInlierProportion();
    csvFile << "," << foeMeasured->getFoE().x;
    csvFile << "," << foeMeasured->getFoE().y;
    csvFile << "," << foeMeasured->getNumberOfInliers();
    csvFile << "," << foeMeasured->getNumberOfParticles();
    csvFile << std::endl;
}

void OwnFlowGrapher::logGoodFrameToCsv(unsigned long long frameNumber, std::shared_ptr<cv::Point2i> foeFiltered, std::shared_ptr<hw::FocusOfExpansionDto> foe, std::shared_ptr<hw::CollisionDetectorResult> detectorResult, double lastDivergence, double avgDivergence)
{
    auto collisionLevel = detectorResult->getCollisionLevel();
    csvFile << frameNumber;
    csvFile << "," << static_cast<int>(collisionLevel);
    csvFile << "," << hw::CollisionLevelHelper::toString(collisionLevel);
    csvFile << "," << detectorResult->getEvaluationResultText();
    csvFile << "," << lastDivergence;
    csvFile << "," << avgDivergence;
    csvFile << "," << foeFiltered->x;
    csvFile << "," << foeFiltered->y;
    csvFile << "," << foe->getInlierProportion();
    csvFile << "," << foe->getFoE().x;
    csvFile << "," << foe->getFoE().y;
    csvFile << "," << foe->getNumberOfInliers();
    csvFile << "," << foe->getNumberOfParticles();
    csvFile << std::endl;
}
