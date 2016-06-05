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

void CollisionAvoidanceTestGoPro::go2()
{
	CollisionAvoidanceSettings& settings = CollisionAvoidanceSettings::getInstance();
	settings.setFileName("/home/helli/d/m/qgroundcontrol/src/CollisionAvoidance/opticalflow/res/rl/go2.mp4");

	CollisionAvoidanceTestSettingsDto dto;
	dto.shouldTriggerCollisionImmanent = true;
	dto.lowerFrameNumberBound = 100;
	dto.upperFrameNumberBound = 215;

	CollisionAvoidanceTestBase::_testCa(settings, qgcApp()->toolbox(), dto);
}

void CollisionAvoidanceTestGoPro::go3()
{
	CollisionAvoidanceSettings& settings = CollisionAvoidanceSettings::getInstance();
	settings.setFileName("/home/helli/d/m/qgroundcontrol/src/CollisionAvoidance/opticalflow/res/rl/go3.mp4");

	CollisionAvoidanceTestSettingsDto dto;
	dto.shouldTriggerCollisionImmanent = true;
	dto.lowerFrameNumberBound = 260;
	dto.upperFrameNumberBound = 280;

	CollisionAvoidanceTestBase::_testCa(settings, qgcApp()->toolbox(), dto);
}

void CollisionAvoidanceTestGoPro::go4()
{
	CollisionAvoidanceSettings& settings = CollisionAvoidanceSettings::getInstance();
	settings.setFileName("/home/helli/d/m/qgroundcontrol/src/CollisionAvoidance/opticalflow/res/rl/go4.mp4");

	CollisionAvoidanceTestSettingsDto dto;
	dto.shouldTriggerCollisionImmanent = true;
	dto.lowerFrameNumberBound = 210;
	dto.upperFrameNumberBound = 230;

	CollisionAvoidanceTestBase::_testCa(settings, qgcApp()->toolbox(), dto);
}

void CollisionAvoidanceTestGoPro::go5()
{
	CollisionAvoidanceSettings& settings = CollisionAvoidanceSettings::getInstance();
	settings.setFileName("/home/helli/d/m/qgroundcontrol/src/CollisionAvoidance/opticalflow/res/rl/go5.mp4");

	CollisionAvoidanceTestSettingsDto dto;
	dto.shouldTriggerCollisionImmanent = true;
	dto.lowerFrameNumberBound = 500;
	dto.upperFrameNumberBound = 700; // TODO: re-check this video, it's only somewhat ok, CA is triggered quite early (but both due to HIGH and avgThreshold)

	CollisionAvoidanceTestBase::_testCa(settings, qgcApp()->toolbox(), dto);
}

void CollisionAvoidanceTestGoPro::go6()
{
	CollisionAvoidanceSettings& settings = CollisionAvoidanceSettings::getInstance();
	settings.setFileName("/home/helli/d/m/qgroundcontrol/src/CollisionAvoidance/opticalflow/res/rl/go6.mp4");

	CollisionAvoidanceTestSettingsDto dto;
	dto.shouldTriggerCollisionImmanent = true;
	dto.lowerFrameNumberBound = 270;
	dto.upperFrameNumberBound = 290;

	CollisionAvoidanceTestBase::_testCa(settings, qgcApp()->toolbox(), dto);
}

void CollisionAvoidanceTestGoPro::go7()
{
	CollisionAvoidanceSettings& settings = CollisionAvoidanceSettings::getInstance();
	settings.setFileName("/home/helli/d/m/qgroundcontrol/src/CollisionAvoidance/opticalflow/res/rl/go7.mp4");

	CollisionAvoidanceTestSettingsDto dto;
	dto.shouldTriggerCollisionImmanent = true;
	dto.lowerFrameNumberBound = 200;
	dto.upperFrameNumberBound = 350;

	CollisionAvoidanceTestBase::_testCa(settings, qgcApp()->toolbox(), dto);
}


void CollisionAvoidanceTestGoPro::gopr0109()
{
	CollisionAvoidanceSettings& settings = CollisionAvoidanceSettings::getInstance();
	settings.setFileName("/home/helli/d/m/qgroundcontrol/src/CollisionAvoidance/opticalflow/res/gopro/simulated/GOPR0109.MP4");

	CollisionAvoidanceTestSettingsDto dto;
	dto.shouldTriggerCollisionImmanent = true;
	dto.lowerFrameNumberBound = 180;
	dto.upperFrameNumberBound = 200;
	// todo it's triggered too early (because of avg threshold) - this video has really many high divergence readings

	CollisionAvoidanceTestBase::_testCa(settings, qgcApp()->toolbox(), dto);
}


void CollisionAvoidanceTestGoPro::gopr0121()
{
	CollisionAvoidanceSettings& settings = CollisionAvoidanceSettings::getInstance();
	settings.setFileName("/home/helli/d/m/qgroundcontrol/src/CollisionAvoidance/opticalflow/res/gopro/air/GOPR0121.MP4");

	CollisionAvoidanceTestSettingsDto dto;
	dto.shouldTriggerCollisionImmanent = false;

	CollisionAvoidanceTestBase::_testCa(settings, qgcApp()->toolbox(), dto);
}

void CollisionAvoidanceTestGoPro::gopr0122()
{
	CollisionAvoidanceSettings& settings = CollisionAvoidanceSettings::getInstance();
	settings.setFileName("/home/helli/d/m/qgroundcontrol/src/CollisionAvoidance/opticalflow/res/gopro/air/GOPR0122.MP4");

	CollisionAvoidanceTestSettingsDto dto;
	dto.shouldTriggerCollisionImmanent = false;

	CollisionAvoidanceTestBase::_testCa(settings, qgcApp()->toolbox(), dto);
}

void CollisionAvoidanceTestGoPro::gopr0124()
{
	CollisionAvoidanceSettings& settings = CollisionAvoidanceSettings::getInstance();
	settings.setFileName("/home/helli/d/m/qgroundcontrol/src/CollisionAvoidance/opticalflow/res/gopro/air/GOPR0124.MP4");

	CollisionAvoidanceTestSettingsDto dto;
	dto.shouldTriggerCollisionImmanent = false;

	CollisionAvoidanceTestBase::_testCa(settings, qgcApp()->toolbox(), dto);
}

void CollisionAvoidanceTestGoPro::gopr0125()
{
	CollisionAvoidanceSettings& settings = CollisionAvoidanceSettings::getInstance();
	settings.setFileName("/home/helli/d/m/qgroundcontrol/src/CollisionAvoidance/opticalflow/res/gopro/air/GOPR0125.MP4");

	CollisionAvoidanceTestSettingsDto dto;
	dto.shouldTriggerCollisionImmanent = false;
	// todo this video has place for improvement

	CollisionAvoidanceTestBase::_testCa(settings, qgcApp()->toolbox(), dto);
}

void CollisionAvoidanceTestGoPro::gopr0131()
{
	CollisionAvoidanceSettings& settings = CollisionAvoidanceSettings::getInstance();
	settings.setFileName("/home/helli/d/m/qgroundcontrol/src/CollisionAvoidance/opticalflow/res/gopro/air/GOPR0131.MP4");

	CollisionAvoidanceTestSettingsDto dto;
	dto.shouldTriggerCollisionImmanent = true;
	dto.lowerFrameNumberBound = 210;
	dto.upperFrameNumberBound = 230;

	CollisionAvoidanceTestBase::_testCa(settings, qgcApp()->toolbox(), dto);
}

void CollisionAvoidanceTestGoPro::gopr0133()
{
	CollisionAvoidanceSettings& settings = CollisionAvoidanceSettings::getInstance();
	settings.setFileName("/home/helli/d/m/qgroundcontrol/src/CollisionAvoidance/opticalflow/res/gopro/air/GOPR0133.MP4");

	CollisionAvoidanceTestSettingsDto dto;
	dto.shouldTriggerCollisionImmanent = false;
	// dto.lowerFrameNumberBound = 350;
	// dto.upperFrameNumberBound = 370;
	// todo little texture, bad optical flow, no CA, NOK 

	CollisionAvoidanceTestBase::_testCa(settings, qgcApp()->toolbox(), dto);
}
