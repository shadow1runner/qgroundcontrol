#include "CollisionAvoidanceTestBoscamSyntheticFisheye.h"

#include "QGCApplication.h"

#include <QDebug>

CollisionAvoidanceTestBoscamSyntheticFisheye::CollisionAvoidanceTestBoscamSyntheticFisheye() 
{

}

void CollisionAvoidanceTestBoscamSyntheticFisheye::init(void)
{
    UnitTest::init();
    _init();

    CollisionAvoidanceSettings& settings = CollisionAvoidanceSettings::getInstance();

    // set Boscam (normal lense) specific camera settings


    settings.DivergenceThreshold = 0.03;
    settings.AvgDivergenceThreshold = 0.02;
    settings.SubsampleAmount = 2; // Boscam has a lower resolution than the GoPro
    settings.WindowSize = 6; // otherwise there would hardly be any frame with big enough inlier ratio
    settings.RawFrameRotation = 0; // don't rotate

    // set Boscam long fisheye with IR fiter specific camera settings
    settings.InlierProportionThreshold = 0.002; // changed from 0.003
    settings.UndistortFrames = true;
    settings.OcamModelPath = "/home/helli/d/m/qgroundcontrol/src/CollisionAvoidance/opticalflow/OCamCalib/calibrationFrames/longFisheye/a3/calib_results480p.txt";    
    settings.reloadOcamModel();
}

void CollisionAvoidanceTestBoscamSyntheticFisheye::cleanup(void)
{
    _cleanup();
}

// void CollisionAvoidanceTestBoscamSyntheticFisheye::_5mHover()
// {
//     // does generate only one `BiggerThanDivInlierRatio` frame
//     CollisionAvoidanceSettings& settings = CollisionAvoidanceSettings::getInstance();
//     settings.setFileName("/home/helli/d/m/qgroundcontrol/src/CollisionAvoidance/opticalflow/res/boscam/fisheye/longFisheye/90degDown/5mHover.avi");

//     CollisionAvoidanceTestSettingsDto dto;
//     dto.shouldTriggerCollisionImmanent = true;
//     dto.lowerFrameNumberBound = 350;
//     dto.upperFrameNumberBound = 380;

//     CollisionAvoidanceTestBase::_testCa(settings, qgcApp()->toolbox(), dto);
// }
