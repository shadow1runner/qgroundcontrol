#include "CollisionAvoidanceTestBoscamLongFisheye.h"

#include "QGCApplication.h"

#include <QDebug>

CollisionAvoidanceTestBoscamLongFisheye::CollisionAvoidanceTestBoscamLongFisheye() 
{

}

void CollisionAvoidanceTestBoscamLongFisheye::init(void)
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

    // set Boscam long fisheye with IR fiter specific camera settings
    settings.UndistortFrames = true;
    settings.OcamModelPath = "/home/helli/d/m/qgroundcontrol/src/CollisionAvoidance/opticalflow/OCamCalib/calibrationFrames/longFisheye/ir_a3_2/calib_results480p.txt";
}

void CollisionAvoidanceTestBoscamLongFisheye::cleanup(void)
{
    _cleanup();
}

void CollisionAvoidanceTestBoscamLongFisheye::fenceCrash()
{
    CollisionAvoidanceSettings& settings = CollisionAvoidanceSettings::getInstance();
    settings.setFileName("/home/helli/d/m/qgroundcontrol/src/CollisionAvoidance/opticalflow/res/boscam/fisheye/longFisheye/fenceCrash.avi");

    CollisionAvoidanceTestSettingsDto dto;
    dto.shouldTriggerCollisionImmanent = true;
    dto.lowerFrameNumberBound = 45;
    dto.upperFrameNumberBound = 60;

    CollisionAvoidanceTestBase::_testCa(settings, qgcApp()->toolbox(), dto);
}