message("Adding Collision Avoidance Dependencies")

HEADERS += \
    $$PWD/opticalflow/BufferedFrameGrabber.h \
    $$PWD/opticalflow/CircularBuffer.h \
    $$PWD/opticalflow/config.h \
    $$PWD/opticalflow/Converter.h \
    $$PWD/opticalflow/Divergence.h \
    $$PWD/opticalflow/DivergenceHistory.h \
    $$PWD/opticalflow/FocusOfExpansionCalculator.h \
    $$PWD/opticalflow/FocusOfExpansionDto.h \
    $$PWD/opticalflow/FramePersister.h \
    $$PWD/opticalflow/helper/QtHelper.h \
    $$PWD/opticalflow/helper/AvgWatch.h \
    $$PWD/opticalflow/helper/ConsoleDisplayer.h \
    $$PWD/opticalflow/helper/Displayer.h \
    $$PWD/opticalflow/helper/DrawHelper.h \
    $$PWD/opticalflow/helper/HeatMap.h \
    $$PWD/opticalflow/helper/ImShower.h \
    $$PWD/opticalflow/helper/Macros.h \
    $$PWD/opticalflow/helper/StopWatch.h \
    $$PWD/opticalflow/helper/ThreadWarmupHelper.h \
    $$PWD/opticalflow/OCamCalib/undistortFunctions/ocam_functions.h \
    $$PWD/opticalflow/Kalman.h \
    $$PWD/opticalflow/OwnFlow.h \
    $$PWD/opticalflow/RandomCollider.h \
    $$PWD/opticalflow/CollisionAvoidanceSettings.h \
    $$PWD/opticalflow/CollisionDetector.h \
    $$PWD/opticalflow/UiFramePreparer.h \
    $$PWD/opticalflow/CollisionLevel.h \
    $$PWD/CollisionAvoidanceController.h \
    $$PWD/OwnFlowHandler.h \
    $$PWD/CollisionAvoidanceDataProvider.h \
    $$PWD/OwnFlowWorker.h \
    $$PWD/CollisionAvoidanceSettingsController.h \
    $$PWD/OwnFlowGrapher.h \
    $$PWD/CollisionActor.h \
    $$PWD/RoiBuilder.h

SOURCES += \
    $$PWD/opticalflow/BufferedFrameGrabber.cpp \
    $$PWD/opticalflow/Converter.cpp \
    $$PWD/opticalflow/Divergence.cpp \
    $$PWD/opticalflow/DivergenceHistory.cpp \
    $$PWD/opticalflow/FocusOfExpansionCalculator.cpp \
    $$PWD/opticalflow/FocusOfExpansionDto.cpp \
    $$PWD/opticalflow/FramePersister.cpp \
    $$PWD/opticalflow/helper/QtHelper.cpp \
    $$PWD/opticalflow/helper/DrawHelper.cpp \
    $$PWD/opticalflow/helper/HeatMap.cpp \
    $$PWD/opticalflow/OCamCalib/undistortFunctions/ocam_functions.cpp \
    $$PWD/opticalflow/Kalman.cpp \
    # $$PWD/opticalflow/main.cpp \
    $$PWD/opticalflow/OwnFlow.cpp \
    $$PWD/opticalflow/RandomCollider.cpp \
    $$PWD/opticalflow/CollisionAvoidanceSettings.cpp \
    $$PWD/opticalflow/CollisionDetector.cpp \
    $$PWD/opticalflow/UiFramePreparer.cpp \
    $$PWD/opticalflow/CollisionLevel.cpp \
    $$PWD/CollisionAvoidanceController.cpp \
    $$PWD/OwnFlowHandler.cpp \
    $$PWD/CollisionAvoidanceDataProvider.cpp \
    $$PWD/OwnFlowWorker.cpp \
    $$PWD/OwnFlowGrapher.cpp \
    $$PWD/CollisionActor.cpp \
    $$PWD/RoiBuilder.cpp

DebugBuild {
    # Tests
    HEADERS += \
        $$PWD/CollisionAvoidanceTestBase.h \
        $$PWD/CollisionAvoidanceTestGoPro.h \
        $$PWD/CollisionAvoidanceTestBoscamNormalLense.h \
        $$PWD/CollisionAvoidanceTestBoscamCheapFisheye.h

    SOURCES += \
        $$PWD/CollisionAvoidanceTestBase.cpp \
        $$PWD/CollisionAvoidanceTestGoPro.cpp \
        $$PWD/CollisionAvoidanceTestBoscamNormalLense.cpp \
        $$PWD/CollisionAvoidanceTestBoscamCheapFisheye.cpp
}

INCLUDEPATH += \
    $$PWD/opticalflow \
    $$PWD/opticalflow/helper \
    $$PWD/opticalflow/OCamCalib/undistortFunctions

#-------------------------------------------------------------------------------------
# OpenCV
INCLUDEPATH += \
    "/usr/include/" \
    "/usr/include/opencv" \
    "/usr/include/opencv2" \
    "/usr/local/include/" \
    "/usr/local/include/opencv" \
    "/usr/local/include/opencv2"

LIBS += \
    -L/usr/local/lib/ \
    -lopencv_core \
    -lopencv_highgui \
    -lopencv_imgproc \
    -lopencv_features2d \
    -lopencv_calib3d \
    -lopencv_flann \
    -lopencv_imgcodecs \
    -lopencv_ml \
    -lopencv_objdetect \
    -lopencv_photo \
    -lopencv_shape \
    -lopencv_stitching \
    -lopencv_superres \
    -lopencv_video \
    -lopencv_videoio \
    -lopencv_videostab
    #-lopencv_optflow

#-------------------------------------------------------------------------------------
# boost - http://stackoverflow.com/a/16998798

DEFINES += BOOST_THREAD_USE_LIB
#DEFINES += BOOST_THREAD_POSIX #https://svn.boost.org/trac/boost/ticket/5964
#DEFINES += BOOST_THREAD_LINUX
DEFINES += BOOST_SYSTEM_NO_DEPRECATED # http://stackoverflow.com/a/18877

INCLUDEPATH += \
    /usr/include/boost

LIBS += \
    -L/usr/lib/ \
    -lboost_system \
    -lboost_thread \
    -lboost_date_time \
    -lboost_program_options \
    -lboost_filesystem \
    -lboost_regex

#-------------------------------------------------------------------------------------

DISTFILES += \
    $$PWD/FlightDisplayViewCollisionAvoidance.qml \
    $$PWD/CollisionAvoidanceSettings.qml \

#-------------------------------------------------------------------------------------

# QMAKE_POST_LINK += echo "Copying Collision Avoidance files"
# QMAKE_POST_LINK += && $$QMAKE_COPY_DIR $$BASEDIR/src/CollisionAvoidance/opticalflow/res $$DESTDIR/CollisionAvoidance/res && 
# && ... more to come (in QGCSetup.pri)
