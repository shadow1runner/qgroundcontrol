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
    dto.upperFrameNumberBound = 170;

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

void CollisionAvoidanceTestBoscamNormalLense::frontalCrash()
{
    CollisionAvoidanceSettings& settings = CollisionAvoidanceSettings::getInstance();
    settings.setFileName("/home/helli/d/m/qgroundcontrol/src/CollisionAvoidance/opticalflow/res/boscam/normalLense/frontalCrash.avi");

    CollisionAvoidanceTestSettingsDto dto;
    dto.shouldTriggerCollisionImmanent = true;
    dto.lowerFrameNumberBound = 80;
    dto.upperFrameNumberBound = 100;

    CollisionAvoidanceTestBase::_testCa(settings, qgcApp()->toolbox(), dto);
}

void CollisionAvoidanceTestBoscamNormalLense::frontalCrash2()
{
    CollisionAvoidanceSettings& settings = CollisionAvoidanceSettings::getInstance();
    settings.setFileName("/home/helli/d/m/qgroundcontrol/src/CollisionAvoidance/opticalflow/res/boscam/normalLense/frontalCrash2.avi");

    CollisionAvoidanceTestSettingsDto dto;
    dto.shouldTriggerCollisionImmanent = true;
    dto.lowerFrameNumberBound = 14;
    dto.upperFrameNumberBound = 25; // todo: this is quite early :S

    CollisionAvoidanceTestBase::_testCa(settings, qgcApp()->toolbox(), dto);
}

void CollisionAvoidanceTestBoscamNormalLense::grillFrontalCrash()
{
    CollisionAvoidanceSettings& settings = CollisionAvoidanceSettings::getInstance();
    settings.setFileName("/home/helli/d/m/qgroundcontrol/src/CollisionAvoidance/opticalflow/res/boscam/normalLense/grillFrontalCrash.avi");

    CollisionAvoidanceTestSettingsDto dto;
    dto.shouldTriggerCollisionImmanent = true;
    dto.lowerFrameNumberBound = 45;
    dto.upperFrameNumberBound = 60;

    CollisionAvoidanceTestBase::_testCa(settings, qgcApp()->toolbox(), dto);
}

void CollisionAvoidanceTestBoscamNormalLense::sideCrash()
{
    CollisionAvoidanceSettings& settings = CollisionAvoidanceSettings::getInstance();
    settings.setFileName("/home/helli/d/m/qgroundcontrol/src/CollisionAvoidance/opticalflow/res/boscam/normalLense/sideCrash.avi");

    CollisionAvoidanceTestSettingsDto dto;
    dto.shouldTriggerCollisionImmanent = true;
    dto.lowerFrameNumberBound = 70;
    dto.upperFrameNumberBound = 80;
    // todo: this video is _very_ hard to detect

    CollisionAvoidanceTestBase::_testCa(settings, qgcApp()->toolbox(), dto);
}

void CollisionAvoidanceTestBoscamNormalLense::hutFrontalCrash()
{
    CollisionAvoidanceSettings& settings = CollisionAvoidanceSettings::getInstance();
    settings.setFileName("/home/helli/d/m/qgroundcontrol/src/CollisionAvoidance/opticalflow/res/boscam/normalLense/hutFrontalCrash.avi");

    CollisionAvoidanceTestSettingsDto dto;
    dto.shouldTriggerCollisionImmanent = true;
    dto.lowerFrameNumberBound = 90;
    dto.upperFrameNumberBound = 260;
    // TODO: this video has very different divergence readings, that's why the above range is _so_ big
    // -> does the algorithm work the same each time? (i.e. is the starting state correct, do the FoE readings correlate between multiple runs)
    // -> how does the black border to the right (camera specific) and the roof influence the FoE?
    // - very interesting video in general

    CollisionAvoidanceTestBase::_testCa(settings, qgcApp()->toolbox(), dto);
}

void CollisionAvoidanceTestBoscamNormalLense::movingBackgroundNoCrash()
{
    CollisionAvoidanceSettings& settings = CollisionAvoidanceSettings::getInstance();
    settings.setFileName("/home/helli/d/m/qgroundcontrol/src/CollisionAvoidance/opticalflow/res/boscam/normalLense/movingBackgroundNoCrash.avi");

    CollisionAvoidanceTestSettingsDto dto;
    dto.shouldTriggerCollisionImmanent = false;

    CollisionAvoidanceTestBase::_testCa(settings, qgcApp()->toolbox(), dto);
}