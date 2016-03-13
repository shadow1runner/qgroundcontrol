message("Adding Collision Avoidance stuff")

HEADERS += \
    $$PWD/opticalflow/BufferedFrameGrabber.h \
    $$PWD/opticalflow/CircularBuffer.h \
    $$PWD/opticalflow/config.h \
    $$PWD/opticalflow/Converter.h \
    $$PWD/opticalflow/Divergence.h \
    $$PWD/opticalflow/FocusOfExpansionCalculator.h \
    $$PWD/opticalflow/FocusOfExpansionDto.h \
    $$PWD/opticalflow/helper/AvgWatch.h \
    $$PWD/opticalflow/helper/ConsoleDisplayer.h \
    $$PWD/opticalflow/helper/Displayer.h \
    $$PWD/opticalflow/helper/DrawHelper.h \
    $$PWD/opticalflow/helper/HeatMap.h \
    $$PWD/opticalflow/helper/ImShower.h \
    $$PWD/opticalflow/helper/Macros.h \
    $$PWD/opticalflow/helper/StopWatch.h \
    $$PWD/opticalflow/helper/ThreadWarmupHelper.h \
    $$PWD/opticalflow/Kalman.h \
    $$PWD/opticalflow/OwnFlow.h \
    $$PWD/opticalflow/RandomCollider.h \
    $$PWD/CollisionAvoidanceController.h \
    $$PWD/CollisionAvoidanceWidget.h \
    $$PWD/OpenCVQuickPaintedItem.h

SOURCES += \
    $$PWD/opticalflow/BufferedFrameGrabber.cpp \
    $$PWD/opticalflow/Converter.cpp \
    $$PWD/opticalflow/Divergence.cpp \
    $$PWD/opticalflow/FocusOfExpansionCalculator.cpp \
    $$PWD/opticalflow/FocusOfExpansionDto.cpp \
    $$PWD/opticalflow/helper/DrawHelper.cpp \
    $$PWD/opticalflow/helper/HeatMap.cpp \
    $$PWD/opticalflow/Kalman.cpp \
    # $$PWD/opticalflow/main.cpp \
    $$PWD/opticalflow/OwnFlow.cpp \
    $$PWD/opticalflow/RandomCollider.cpp \
    $$PWD/CollisionAvoidanceController.cpp \
    $$PWD/CollisionAvoidanceWidget.cpp \
    $$PWD/OpenCVQuickPaintedItem.cpp


#-------------------------------------------------------------------------------------
# OpenCV
INCLUDEPATH += \
    "/usr/include/" \
    "/usr/include/opencv" \
    "/usr/include/opencv2"

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
    $$PWD/CollisionAvoidance.qml \
    $$PWD/FlightDisplayViewCollisionAvoidance.qml
