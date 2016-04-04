#include "OwnFlowWorker.h"
#include "OwnFlowHandler.h"

OwnFlowWorker::OwnFlowWorker(const std::string& fileName, OwnFlowHandler* const ownFlowHandler)
    : QObject((QObject*)ownFlowHandler)
	, _fileName(fileName)
{ 
    connect(ownFlowHandler, &OwnFlowHandler::startTriggered,
            this,           &OwnFlowWorker::start);
}

void OwnFlowWorker::start() {
    qDebug() << "Starting OwnFlowWorker on Thread" << QThread::currentThreadId();
    hw::BufferedFrameGrabber frame_grabber(_fileName, 1, [](cv::Mat input) {return input;});

    OwnFlowHandler * const ownFlowHandler = (OwnFlowHandler* const)parent();

	// initialize baseFrame
    ownFlowHandler->Converter()->convertImage(frame_grabber.next());


    while(frame_grabber.has_next()) {
        auto currentFrame = frame_grabber.next();
        ownFlowHandler->Converter()->convertImage(currentFrame);
	}
}

void OwnFlowWorker::stop() {	
} 
