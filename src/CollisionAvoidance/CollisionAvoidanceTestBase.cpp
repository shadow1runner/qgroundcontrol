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
	OwnFlowWorker worker(settings, toolbox);
    auto* ownFlow = worker.ownFlow();

    auto caTriggered = false;

    // make sure that the latest & gratest ocam model is used (if `UndistortFrames` is true)
    settings.reloadOcamModel();

    connect(ownFlow, &hw::OwnFlow::collisionImmanent,
            this, [ownFlow, this, &settings, dto, &caTriggered] (const cv::Mat& frame, unsigned long long frameNumber, std::shared_ptr<cv::Point2i> foeFiltered, std::shared_ptr<hw::FocusOfExpansionDto> foe, std::shared_ptr<hw::CollisionDetectorResult> detectorResult, double lastDivergence, double avgDivergence) {

                caTriggered = true;

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

	worker.start();

    // sleep(1); // wait for the lambda above to be invoked (if at all)
    // QVERIFY(caTriggered == dto.shouldTriggerCollisionImmanent);
}
