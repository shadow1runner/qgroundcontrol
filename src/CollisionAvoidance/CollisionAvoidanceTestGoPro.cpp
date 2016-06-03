#include "CollisionAvoidanceTestGoPro.h"

#include "QGCApplication.h"

#include <QDebug>

CollisionAvoidanceTestGoPro::CollisionAvoidanceTestGoPro() 
{

}

void CollisionAvoidanceTestGoPro::init(void)
{
    UnitTest::init();
    _init();

    CollisionAvoidanceSettings& settings = CollisionAvoidanceSettings::getInstance();

    // set GoPro Hero3 specific camera settings
	settings.DivergenceThreshold = 0.20;
	settings.AvgDivergenceThreshold = 0.16;
	settings.SubsampleAmount = 5;
	settings.WindowSize = 4;
	settings.RawFrameRotation = 0;
}

void CollisionAvoidanceTestGoPro::cleanup(void)
{
    _cleanup();
}

void CollisionAvoidanceTestGoPro::go2(void)
{
	_expectMissedMessageBox = true;

	CollisionAvoidanceSettings& settings = CollisionAvoidanceSettings::getInstance();
    settings.setFileName("/home/helli/d/m/qgroundcontrol/src/CollisionAvoidance/opticalflow/res/rl/go2.mp4");


	CollisionAvoidanceTestSettingsDto dto;
	dto.shouldTriggerCollisionImmanent = true;
	dto.lowerFrameNumberBound = 200;
	dto.upperFrameNumberBound = 215;

	CollisionAvoidanceTestBase::_testCa(settings, qgcApp()->toolbox(), dto);
}
