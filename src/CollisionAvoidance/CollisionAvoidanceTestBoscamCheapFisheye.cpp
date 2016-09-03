#include "CollisionAvoidanceTestBoscamCheapFisheye.h"

#include "QGCApplication.h"

#include <QDebug>

CollisionAvoidanceTestBoscamCheapFisheye::CollisionAvoidanceTestBoscamCheapFisheye() 
{

}

void CollisionAvoidanceTestBoscamCheapFisheye::init(void)
{
    UnitTest::init();
    _init();

    CollisionAvoidanceSettings& settings = CollisionAvoidanceSettings::getInstance();

    // settings for affine divergence
    settings.DivergenceThreshold = 0.03;
    settings.AvgDivergenceThreshold = 0.02;

    // settings for hw::Divergence
    // settings.DivergenceThreshold = 0.20;
    // settings.AvgDivergenceThreshold = 0.16;

    // set Boscam (normal lense) specific camera settings
    settings.SubsampleAmount = 2; // Boscam has a lower resolution than the GoPro
    settings.WindowSize = 6; // otherwise there would hardly be any frame with big enough inlier ratio
    settings.RawFrameRotation = 1; // rotate 90 degree right

    // set Boscam fisheye with IR fiter specific camera settings
    settings.UndistortFrames = true;
    settings.OcamModelPath = "/home/helli/d/m/qgroundcontrol/src/CollisionAvoidance/opticalflow/OCamCalib/calibrationFrames/cheapFisheye/ir_a3_2/calib_results480p.txt";
}

void CollisionAvoidanceTestBoscamCheapFisheye::cleanup(void)
{
    _cleanup();
}

void CollisionAvoidanceTestBoscamCheapFisheye::fenceCrash()
{
    CollisionAvoidanceSettings& settings = CollisionAvoidanceSettings::getInstance();
    settings.setFileName("/home/helli/d/m/qgroundcontrol/src/CollisionAvoidance/opticalflow/res/boscam/fisheye/cheapFisheye/fenceCrash.avi");

    CollisionAvoidanceTestSettingsDto dto;
    dto.shouldTriggerCollisionImmanent = true;
    dto.lowerFrameNumberBound = 45;
    dto.upperFrameNumberBound = 60;

    CollisionAvoidanceTestBase::_testCa(settings, qgcApp()->toolbox(), dto);
}

void CollisionAvoidanceTestBoscamCheapFisheye::fenceCrash2()
{
    CollisionAvoidanceSettings& settings = CollisionAvoidanceSettings::getInstance();
    settings.setFileName("/home/helli/d/m/qgroundcontrol/src/CollisionAvoidance/opticalflow/res/boscam/fisheye/cheapFisheye/fenceCrash.avi");

    CollisionAvoidanceTestSettingsDto dto;
    dto.shouldTriggerCollisionImmanent = true;
    // TODO: this is a really BAD video for CA (or better said how the algorithm behaves here :S - investigate more; there are only ~6 frames with ok-ish inlier ration and they account for CA in a too late manner
    dto.lowerFrameNumberBound = 140;
    dto.upperFrameNumberBound = 160;

    CollisionAvoidanceTestBase::_testCa(settings, qgcApp()->toolbox(), dto);
}

void CollisionAvoidanceTestBoscamCheapFisheye::roofThinObstacle()
{
    CollisionAvoidanceSettings& settings = CollisionAvoidanceSettings::getInstance();
    settings.setFileName("/home/helli/d/m/qgroundcontrol/src/CollisionAvoidance/opticalflow/res/boscam/fisheye/cheapFisheye/roofThinObstacle.avi");

    CollisionAvoidanceTestSettingsDto dto;
    dto.shouldTriggerCollisionImmanent = true;
    // surprisingly this is an awesome result and the UAV would just have stopped in time :)
    dto.lowerFrameNumberBound = 180;
    dto.upperFrameNumberBound = 219;

    CollisionAvoidanceTestBase::_testCa(settings, qgcApp()->toolbox(), dto);
}

void CollisionAvoidanceTestBoscamCheapFisheye::groundRaisesCa()
{
    CollisionAvoidanceSettings& settings = CollisionAvoidanceSettings::getInstance();
    settings.setFileName("/home/helli/d/m/qgroundcontrol/src/CollisionAvoidance/opticalflow/res/boscam/fisheye/cheapFisheye/groundRaisesCa.avi");

    CollisionAvoidanceTestSettingsDto dto;
    dto.shouldTriggerCollisionImmanent = true;
    dto.lowerFrameNumberBound = 200;
    dto.upperFrameNumberBound = 460;

    CollisionAvoidanceTestBase::_testCa(settings, qgcApp()->toolbox(), dto);
}

void CollisionAvoidanceTestBoscamCheapFisheye::groundHoveringNoCa()
{
    CollisionAvoidanceSettings& settings = CollisionAvoidanceSettings::getInstance();
    settings.setFileName("/home/helli/d/m/qgroundcontrol/src/CollisionAvoidance/opticalflow/res/boscam/fisheye/cheapFisheye/groundHoveringNoCa.avi");

    CollisionAvoidanceTestSettingsDto dto;
    dto.shouldTriggerCollisionImmanent = false;

    CollisionAvoidanceTestBase::_testCa(settings, qgcApp()->toolbox(), dto);
}

void CollisionAvoidanceTestBoscamCheapFisheye::_5mHoveringNoCa()
{
    CollisionAvoidanceSettings& settings = CollisionAvoidanceSettings::getInstance();
    settings.setFileName("/home/helli/d/m/qgroundcontrol/src/CollisionAvoidance/opticalflow/res/boscam/fisheye/cheapFisheye/5mHoveringNoCa.avi");

    CollisionAvoidanceTestSettingsDto dto;
    dto.shouldTriggerCollisionImmanent = false;

    CollisionAvoidanceTestBase::_testCa(settings, qgcApp()->toolbox(), dto);
}

void CollisionAvoidanceTestBoscamCheapFisheye::fastFenceFlyOver()
{
    CollisionAvoidanceSettings& settings = CollisionAvoidanceSettings::getInstance();
    settings.setFileName("/home/helli/d/m/qgroundcontrol/src/CollisionAvoidance/opticalflow/res/boscam/fisheye/cheapFisheye/fastFenceFlyOver.avi");

    CollisionAvoidanceTestSettingsDto dto;
    dto.shouldTriggerCollisionImmanent = true;
    // this detection works surprisingly WELL, considering the high speed Volcy would still have been able to stop in time :-)
    dto.lowerFrameNumberBound = 50;
    dto.upperFrameNumberBound = 70;

    CollisionAvoidanceTestBase::_testCa(settings, qgcApp()->toolbox(), dto);
}

void CollisionAvoidanceTestBoscamCheapFisheye::fastMetalFence()
{
    CollisionAvoidanceSettings& settings = CollisionAvoidanceSettings::getInstance();
    settings.setFileName("/home/helli/d/m/qgroundcontrol/src/CollisionAvoidance/opticalflow/res/boscam/fisheye/cheapFisheye/fastMetalFence.avi");

    CollisionAvoidanceTestSettingsDto dto;
    dto.shouldTriggerCollisionImmanent = true;
    dto.lowerFrameNumberBound = 68;
    dto.upperFrameNumberBound = 86;

    CollisionAvoidanceTestBase::_testCa(settings, qgcApp()->toolbox(), dto);
}

