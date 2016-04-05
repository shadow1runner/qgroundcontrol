#include "OwnFlowWorker.h"
#include "OwnFlowHandler.h"

OwnFlowWorker::OwnFlowWorker(const std::string& fileName, const OwnFlowHandler* const ownFlowHandler)
    : QObject(NULL)
	, _fileName(fileName)
    , _ownFlowHandler(ownFlowHandler)
{ 
}

void OwnFlowWorker::start() 
{
    qDebug() << "Starting OwnFlowWorker on Thread" << QThread::currentThreadId();
    hw::BufferedFrameGrabber frame_grabber(_fileName, 1, [](cv::Mat input) {return input;});

	// initialize baseFrame
    _ownFlowHandler->Converter()->convertImage(frame_grabber.next());

    while(frame_grabber.has_next()) {
        auto currentFrame = frame_grabber.next();
        _ownFlowHandler->Converter()->convertImage(currentFrame);
	}
}

void OwnFlowWorker::stop() 
{	
} 
