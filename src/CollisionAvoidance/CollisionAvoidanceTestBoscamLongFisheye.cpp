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
    settings.InlierProportionThreshold = 0.002; // changed from 0.003
    settings.UndistortFrames = true;
    settings.OcamModelPath = "/home/helli/d/m/qgroundcontrol/src/CollisionAvoidance/opticalflow/OCamCalib/calibrationFrames/longFisheye/a3/calib_results480p.txt";    
    settings.reloadOcamModel();
}

void CollisionAvoidanceTestBoscamLongFisheye::cleanup(void)
{
    _cleanup();
}

void CollisionAvoidanceTestBoscamLongFisheye::_5mHover()
{
    // does generate only one `BiggerThanDivInlierRatio` frame
    CollisionAvoidanceSettings& settings = CollisionAvoidanceSettings::getInstance();
    settings.setFileName("/home/helli/d/m/qgroundcontrol/src/CollisionAvoidance/opticalflow/res/boscam/fisheye/longFisheye/90degDown/5mHover.avi");

    CollisionAvoidanceTestSettingsDto dto;
    dto.shouldTriggerCollisionImmanent = true;
    dto.lowerFrameNumberBound = 350;
    dto.upperFrameNumberBound = 380;

    CollisionAvoidanceTestBase::_testCa(settings, qgcApp()->toolbox(), dto);
}

void CollisionAvoidanceTestBoscamLongFisheye::fastFenceOverfly()
{
    CollisionAvoidanceSettings& settings = CollisionAvoidanceSettings::getInstance();
    settings.setFileName("/home/helli/d/m/qgroundcontrol/src/CollisionAvoidance/opticalflow/res/boscam/fisheye/longFisheye/90degDown/fastFenceOverfly.avi");

    CollisionAvoidanceTestSettingsDto dto;
    dto.shouldTriggerCollisionImmanent = false;

    CollisionAvoidanceTestBase::_testCa(settings, qgcApp()->toolbox(), dto);
}

void CollisionAvoidanceTestBoscamLongFisheye::fenceCrash2()
{
    CollisionAvoidanceSettings& settings = CollisionAvoidanceSettings::getInstance();
    settings.setFileName("/home/helli/d/m/qgroundcontrol/src/CollisionAvoidance/opticalflow/res/boscam/fisheye/longFisheye/90degDown/fenceCrash2.avi");

    CollisionAvoidanceTestSettingsDto dto;
    dto.shouldTriggerCollisionImmanent = true;
    dto.lowerFrameNumberBound = 45;
    dto.upperFrameNumberBound = 60;

    CollisionAvoidanceTestBase::_testCa(settings, qgcApp()->toolbox(), dto);
}

void CollisionAvoidanceTestBoscamLongFisheye::fenceCrash()
{
    CollisionAvoidanceSettings& settings = CollisionAvoidanceSettings::getInstance();
    settings.setFileName("/home/helli/d/m/qgroundcontrol/src/CollisionAvoidance/opticalflow/res/boscam/fisheye/longFisheye/90degDown/fenceCrash.avi");

    CollisionAvoidanceTestSettingsDto dto;
    dto.shouldTriggerCollisionImmanent = true;
    dto.lowerFrameNumberBound = 45;
    dto.upperFrameNumberBound = 90;

    CollisionAvoidanceTestBase::_testCa(settings, qgcApp()->toolbox(), dto);
}

void CollisionAvoidanceTestBoscamLongFisheye::fenceHover2()
{
    CollisionAvoidanceSettings& settings = CollisionAvoidanceSettings::getInstance();
    settings.setFileName("/home/helli/d/m/qgroundcontrol/src/CollisionAvoidance/opticalflow/res/boscam/fisheye/longFisheye/90degDown/fenceHover2.avi");

    CollisionAvoidanceTestSettingsDto dto;
    dto.shouldTriggerCollisionImmanent = true;
    dto.lowerFrameNumberBound = 20; // TODO usually triggers at 23 or 24, *very* early :S
    dto.upperFrameNumberBound = 90;

    CollisionAvoidanceTestBase::_testCa(settings, qgcApp()->toolbox(), dto);
}

void CollisionAvoidanceTestBoscamLongFisheye::fenceHover()
{
    CollisionAvoidanceSettings& settings = CollisionAvoidanceSettings::getInstance();
    settings.setFileName("/home/helli/d/m/qgroundcontrol/src/CollisionAvoidance/opticalflow/res/boscam/fisheye/longFisheye/90degDown/fenceHover.avi");

    CollisionAvoidanceTestSettingsDto dto;
    dto.shouldTriggerCollisionImmanent = true;
    dto.lowerFrameNumberBound = 45;
    dto.upperFrameNumberBound = 60;

    CollisionAvoidanceTestBase::_testCa(settings, qgcApp()->toolbox(), dto);
}

void CollisionAvoidanceTestBoscamLongFisheye::houseCrash()
{
    CollisionAvoidanceSettings& settings = CollisionAvoidanceSettings::getInstance();
    settings.setFileName("/home/helli/d/m/qgroundcontrol/src/CollisionAvoidance/opticalflow/res/boscam/fisheye/longFisheye/90degDown/houseCrash.avi");

    CollisionAvoidanceTestSettingsDto dto;
    dto.shouldTriggerCollisionImmanent = true;
    dto.lowerFrameNumberBound = 45;
    dto.upperFrameNumberBound = 60;

    CollisionAvoidanceTestBase::_testCa(settings, qgcApp()->toolbox(), dto);
}

void CollisionAvoidanceTestBoscamLongFisheye::balconyCrash()
{
    CollisionAvoidanceSettings& settings = CollisionAvoidanceSettings::getInstance();
    settings.setFileName("/home/helli/d/m/qgroundcontrol/src/CollisionAvoidance/opticalflow/res/boscam/fisheye/longFisheye/90degDown/balconyCrash.avi");

    CollisionAvoidanceTestSettingsDto dto;
    dto.shouldTriggerCollisionImmanent = true;
    dto.lowerFrameNumberBound = 60;
    dto.upperFrameNumberBound = 85;

    CollisionAvoidanceTestBase::_testCa(settings, qgcApp()->toolbox(), dto);
}

// void CollisionAvoidanceTestBoscamLongFisheye::_45deg_5mHover()
// {
//     CollisionAvoidanceSettings& settings = CollisionAvoidanceSettings::getInstance();
//     settings.setFileName("/home/helli/d/m/qgroundcontrol/src/CollisionAvoidance/opticalflow/res/boscam/fisheye/longFisheye/45degDown/5mHover.avi");

//     CollisionAvoidanceTestSettingsDto dto;
//     dto.shouldTriggerCollisionImmanent = true;
//     dto.lowerFrameNumberBound = 45;
//     dto.upperFrameNumberBound = 60;

//     CollisionAvoidanceTestBase::_testCa(settings, qgcApp()->toolbox(), dto);    
// }

// void CollisionAvoidanceTestBoscamLongFisheye::_45deg_fastFenceOverfly()
// {
//     CollisionAvoidanceSettings& settings = CollisionAvoidanceSettings::getInstance();
//     settings.setFileName("/home/helli/d/m/qgroundcontrol/src/CollisionAvoidance/opticalflow/res/boscam/fisheye/longFisheye/45degDown/fastFenceOverfly.avi");

//     CollisionAvoidanceTestSettingsDto dto;
//     dto.shouldTriggerCollisionImmanent = true;
//     dto.lowerFrameNumberBound = 45;
//     dto.upperFrameNumberBound = 60;

//     CollisionAvoidanceTestBase::_testCa(settings, qgcApp()->toolbox(), dto);    
// }

// void CollisionAvoidanceTestBoscamLongFisheye::_45deg_fenceCrash2()
// {
//     CollisionAvoidanceSettings& settings = CollisionAvoidanceSettings::getInstance();
//     settings.setFileName("/home/helli/d/m/qgroundcontrol/src/CollisionAvoidance/opticalflow/res/boscam/fisheye/longFisheye/45degDown/fenceCrash2.avi");

//     CollisionAvoidanceTestSettingsDto dto;
//     dto.shouldTriggerCollisionImmanent = true;
//     dto.lowerFrameNumberBound = 45;
//     dto.upperFrameNumberBound = 60;

//     CollisionAvoidanceTestBase::_testCa(settings, qgcApp()->toolbox(), dto);    
// }

void CollisionAvoidanceTestBoscamLongFisheye::_45deg_fenceCrash3()
{
    CollisionAvoidanceSettings& settings = CollisionAvoidanceSettings::getInstance();
    settings.setFileName("/home/helli/d/m/qgroundcontrol/src/CollisionAvoidance/opticalflow/res/boscam/fisheye/longFisheye/45degDown/fenceCrash3.avi");

    CollisionAvoidanceTestSettingsDto dto;
    dto.shouldTriggerCollisionImmanent = true;
    dto.lowerFrameNumberBound = 100;
    dto.upperFrameNumberBound = 120;

    CollisionAvoidanceTestBase::_testCa(settings, qgcApp()->toolbox(), dto);    
}

// void CollisionAvoidanceTestBoscamLongFisheye::_45deg_fenceCrash()
// {
//     CollisionAvoidanceSettings& settings = CollisionAvoidanceSettings::getInstance();
//     settings.setFileName("/home/helli/d/m/qgroundcontrol/src/CollisionAvoidance/opticalflow/res/boscam/fisheye/longFisheye/45degDown/fenceCrash.avi");

//     CollisionAvoidanceTestSettingsDto dto;
//     dto.shouldTriggerCollisionImmanent = true;
//     dto.lowerFrameNumberBound = 45;
//     dto.upperFrameNumberBound = 60;

//     CollisionAvoidanceTestBase::_testCa(settings, qgcApp()->toolbox(), dto);    
// }

// void CollisionAvoidanceTestBoscamLongFisheye::_45deg_houseHover2()
// {
//     CollisionAvoidanceSettings& settings = CollisionAvoidanceSettings::getInstance();
//     settings.setFileName("/home/helli/d/m/qgroundcontrol/src/CollisionAvoidance/opticalflow/res/boscam/fisheye/longFisheye/45degDown/houseHover2.avi");

//     CollisionAvoidanceTestSettingsDto dto;
//     dto.shouldTriggerCollisionImmanent = true;
//     dto.lowerFrameNumberBound = 45;
//     dto.upperFrameNumberBound = 60;

//     CollisionAvoidanceTestBase::_testCa(settings, qgcApp()->toolbox(), dto);    
// }

// void CollisionAvoidanceTestBoscamLongFisheye::_45deg_houseHover()
// {
//     CollisionAvoidanceSettings& settings = CollisionAvoidanceSettings::getInstance();
//     settings.setFileName("/home/helli/d/m/qgroundcontrol/src/CollisionAvoidance/opticalflow/res/boscam/fisheye/longFisheye/45degDown/houseHover.avi");

//     CollisionAvoidanceTestSettingsDto dto;
//     dto.shouldTriggerCollisionImmanent = true;
//     dto.lowerFrameNumberBound = 45;
//     dto.upperFrameNumberBound = 60;

//     CollisionAvoidanceTestBase::_testCa(settings, qgcApp()->toolbox(), dto);    
// }

// void CollisionAvoidanceTestBoscamLongFisheye::_45deg_noCaHover()
// {
//     CollisionAvoidanceSettings& settings = CollisionAvoidanceSettings::getInstance();
//     settings.setFileName("/home/helli/d/m/qgroundcontrol/src/CollisionAvoidance/opticalflow/res/boscam/fisheye/longFisheye/45degDown/noCaHover.avi");

//     CollisionAvoidanceTestSettingsDto dto;
//     dto.shouldTriggerCollisionImmanent = true;
//     dto.lowerFrameNumberBound = 45;
//     dto.upperFrameNumberBound = 60;

//     CollisionAvoidanceTestBase::_testCa(settings, qgcApp()->toolbox(), dto);    
// }

// void CollisionAvoidanceTestBoscamLongFisheye::_45deg_slowFenceOverfly()
// {
//     CollisionAvoidanceSettings& settings = CollisionAvoidanceSettings::getInstance();
//     settings.setFileName("/home/helli/d/m/qgroundcontrol/src/CollisionAvoidance/opticalflow/res/boscam/fisheye/longFisheye/45degDown/slowFenceOverfly.avi");

//     CollisionAvoidanceTestSettingsDto dto;
//     dto.shouldTriggerCollisionImmanent = true;
//     dto.lowerFrameNumberBound = 45;
//     dto.upperFrameNumberBound = 60;

//     CollisionAvoidanceTestBase::_testCa(settings, qgcApp()->toolbox(), dto);    

// }
