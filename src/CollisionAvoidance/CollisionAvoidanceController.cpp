#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/imgproc.hpp>
//#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <string>
#include <unistd.h>
#include <chrono>
#include <thread>

#include "CollisionAvoidanceController.h"
#include "OwnFlowHandler.h"
#include "UAS.h"
#include "QGCApplication.h"
#include "BufferedFrameGrabber.h"
#include "config.h"
#include "OwnFlow.h"
#include "Converter.h"
#include "FocusOfExpansionDto.h"
#include "StopWatch.h"
#include "AvgWatch.h"
#include "Macros.h"
#include "Displayer.h"
#include "ConsoleDisplayer.h"
#include "QtHelper.h"

#include <QSettings>
#include <QUrl>
#include <QDebug>
#include <QThread>

CollisionAvoidanceController::CollisionAvoidanceController(QObject* parent)
    : QObject(parent)
    , _hasCollisionAvoidanceStream(true)
    , _ownFlowHandler(NULL)
{
    _ownFlowHandler = qgcApp()->toolbox()->ownFlowHandler();
}

void CollisionAvoidanceController::setHasCollisionAvoidanceStream(bool newValue)
{
	if(_hasCollisionAvoidanceStream!=newValue) {
		_hasCollisionAvoidanceStream = newValue;
		emit hasCollisionAvoidanceStreamChanged(newValue);
	}
}

void CollisionAvoidanceController::onStartStopClicked(bool start)
{
    if(start) {
        qDebug() << "Starting/Resuming OwnFlow on Thread" << QThread::currentThreadId();
        _ownFlowHandler->start();
    } else {
        qDebug() << "Stopping/Pausing OwnFlow on Thread" << QThread::currentThreadId();
        _ownFlowHandler->stop();
    }
}
