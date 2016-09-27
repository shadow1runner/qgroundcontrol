#include "CollisionAvoidanceTestBase.h"

#include "OwnFlowWorker.h"
#include "OwnFlow.h"

#include "LinkManager.h"
#ifdef QT_DEBUG
#include "MockLink.h"
#endif
#include "MultiVehicleManager.h"
#include "QGCApplication.h"

using namespace std;
using namespace hw;

CollisionAvoidanceTestBase::CollisionAvoidanceTestBase()
{

}

void CollisionAvoidanceTestBase::_init()
{
    _expectMissedMessageBox = true;
    
	_connectMockLink(MAV_AUTOPILOT_GENERIC);

    // general settings for all unit tests - can be overriden in derived classes as necessary
    CollisionAvoidanceSettings& settings = CollisionAvoidanceSettings::getInstance();
    settings.WriteToOutputEnabled = false;
    settings.UndistortFrames = false;
    settings.ClearOldFramesEnabled = false;
    settings.InlierProportionThresholdEnabled = true;
    settings.InlierProportionThreshold = 0.003;

    settings.setUseRecordedVideoInsteadOfDevice(true);

    settings.SubsampleAmount = 5;
    settings.Particles = 200000;

    settings.DivergencePatchSize = 20;
    settings.DivergenceThreshold = 0.05;
    settings.DivergenceHistoryBufferSize = 15;

    settings.RoiEnabled = false;
}

void CollisionAvoidanceTestBase::_cleanup()
{
    UnitTest::cleanup();
}

void CollisionAvoidanceTestBase::_testCa(CollisionAvoidanceSettings& settings, QGCToolbox* toolbox, CollisionAvoidanceTestSettingsDto& dto)
{
    _dto = &dto;
	OwnFlowWorker worker(settings, toolbox);
    connect(&worker, &OwnFlowWorker::isPausedChanged, this, &CollisionAvoidanceTestBase::isPausedChanged);

    auto* ownFlow = worker.ownFlow();
    auto* bufferedFrameGrabber = worker.bufferedFrameGrabber();

    _numberOfFrames = bufferedFrameGrabber->get_length_in_frames();
    qDebug() << "Total number of frames in sequence: " << _numberOfFrames;

    _caTriggered = false;

    // make sure that the latest & gratest ocam model is used (if `UndistortFrames` is true)
    settings.reloadOcamModel();

    connect(ownFlow, &hw::OwnFlow::collisionImmanent,
            this, [ownFlow, this, &settings, dto] (const cv::Mat& frame, unsigned long long frameNumber, std::shared_ptr<cv::Point2i> foeFiltered, std::shared_ptr<hw::FocusOfExpansionDto> foe, std::shared_ptr<hw::CollisionDetectorResult> detectorResult, double lastDivergence, double avgDivergence) {

                this->_caTriggered = true;

            	Q_UNUSED(frame);
            	Q_UNUSED(foeFiltered);
            	Q_UNUSED(foe);

                Q_UNUSED(lastDivergence);
            	
            	qDebug() << "collisionImmanent was raised at frame #" << frameNumber;
                QVERIFY(dto.shouldTriggerCollisionImmanent);

            	qDebug() << " testing if it's in range [" << dto.lowerFrameNumberBound << ", " << dto.upperFrameNumberBound << "]";
                QVERIFY(frameNumber >= dto.lowerFrameNumberBound);
                QVERIFY(frameNumber <= dto.upperFrameNumberBound);

                if(detectorResult->getEvaluationResult()==hw::CollisionDetectorResultEnum::TooHighAvgDivThreshold)
                    qDebug() << "CA was triggered to overcoming avg divergence threshold (but could also have been due to reaching `HIGH` normally";
				// succeed
			});

    connect(ownFlow, &hw::OwnFlow::frameSkipped,
            this, [this](const cv::Mat& currentFrame, unsigned long long skipFrameCount, unsigned long long totalFrameCount, std::shared_ptr<hw::FocusOfExpansionDto> foeMeasured, double avgDivergence, const cv::Mat& convertedColorFrame) {
                Q_UNUSED(currentFrame);
                Q_UNUSED(skipFrameCount);
                Q_UNUSED(foeMeasured);
                Q_UNUSED(avgDivergence);
                Q_UNUSED(convertedColorFrame);

                if(totalFrameCount==_numberOfFrames-2)
                    QVERIFY(_caTriggered == this->_dto->shouldTriggerCollisionImmanent);
            });

    connect(ownFlow, &hw::OwnFlow::collisionLevelRatingReady,
            this, [this](const cv::Mat& frame, unsigned long long frameNumber, std::shared_ptr<cv::Point2i> foeFiltered, std::shared_ptr<hw::FocusOfExpansionDto> foe, std::shared_ptr<hw::CollisionDetectorResult> detectorResult, double lastDivergence, double avgDivergence, const cv::Mat& convertedColorFrame) {
                Q_UNUSED(frame);
                Q_UNUSED(foeFiltered);
                Q_UNUSED(foe);
                Q_UNUSED(detectorResult);
                Q_UNUSED(lastDivergence);
                Q_UNUSED(avgDivergence);
                Q_UNUSED(convertedColorFrame);

                if(frameNumber==_numberOfFrames-2)
                    QVERIFY(_caTriggered == this->_dto->shouldTriggerCollisionImmanent);
            });
    

	worker.start();

    // sleep(1); // wait for the lambda above to be invoked (if at all)
    // QVERIFY(caTriggered == dto.shouldTriggerCollisionImmanent);
}

void CollisionAvoidanceTestBase::isPausedChanged(bool isPaused)
{
    if(isPaused) {
        QVERIFY(_caTriggered == _dto->shouldTriggerCollisionImmanent);
    }
}
