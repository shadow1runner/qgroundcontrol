#include "CollisionAvoidanceTestBoscamNormalLense.h"

#include "QGCApplication.h"

#include <QDebug>

CollisionAvoidanceTestBoscamNormalLense::CollisionAvoidanceTestBoscamNormalLense() 
{

}

void CollisionAvoidanceTestBoscamNormalLense::init(void)
{
	UnitTest::init();
	_init();

	CollisionAvoidanceSettings& settings = CollisionAvoidanceSettings::getInstance();

	// set Boscam (normal lense) specific camera settings
	settings.DivergenceThreshold = 0.20;
	settings.AvgDivergenceThreshold = 0.16;
	settings.SubsampleAmount = 2; // Boscam has a lower resolution than the GoPro
	settings.WindowSize = 6; // otherwise there would hardly be any frame with big enough inlier ratio
	settings.RawFrameRotation = 1; // rotate 90 degree right
}

void CollisionAvoidanceTestBoscamNormalLense::cleanup(void)
{
	_cleanup();
}

void CollisionAvoidanceTestBoscamNormalLense::bottomCrash()
{
	CollisionAvoidanceSettings& settings = CollisionAvoidanceSettings::getInstance();
	settings.setFileName("/home/helli/d/m/qgroundcontrol/src/CollisionAvoidance/opticalflow/res/boscam/normalLense/bottomCrash.avi");

	CollisionAvoidanceTestSettingsDto dto;
	dto.shouldTriggerCollisionImmanent = true;
    dto.lowerFrameNumberBound = 140;
    dto.upperFrameNumberBound = 160;

	CollisionAvoidanceTestBase::_testCa(settings, qgcApp()->toolbox(), dto);
}

void CollisionAvoidanceTestBoscamNormalLense::fenceCrash()
{
	CollisionAvoidanceSettings& settings = CollisionAvoidanceSettings::getInstance();
	settings.setFileName("/home/helli/d/m/qgroundcontrol/src/CollisionAvoidance/opticalflow/res/boscam/normalLense/fenceCrash.avi");

	CollisionAvoidanceTestSettingsDto dto;
	dto.shouldTriggerCollisionImmanent = true;
    dto.lowerFrameNumberBound = 70;
    dto.upperFrameNumberBound = 80;

	CollisionAvoidanceTestBase::_testCa(settings, qgcApp()->toolbox(), dto);
}
