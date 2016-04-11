#include "OwnFlowHandler.h"
#include "OwnFlowWorker.h"

#include <QThread>

#include "QGCToolbox.h"
#include "QtHelper.h"

OwnFlowHandler::OwnFlowHandler(QGCApplication* app)
    : QGCTool(app)
{
    QtHelper::registerMetaTypes();
    _ownFlowWorkerThread.setObjectName("OwnFlowHandler");
}

OwnFlowHandler::~OwnFlowHandler()
{
	stop();

    delete _ownFlowWorker;
}

void OwnFlowHandler::setToolbox(QGCToolbox* toolbox)
{
	QGCTool::setToolbox(toolbox);

    _ownFlowWorker = new OwnFlowWorker(_settings, toolbox);
    _ownFlowWorker->moveToThread(&_ownFlowWorkerThread);
    _ownFlowWorkerThread.start();
}

OwnFlowWorker* OwnFlowHandler::ownFlowWorker() 
{
    return _ownFlowWorker;
}


void OwnFlowHandler::start()
{
    // invoke the method in the thread OwnFlowWorker resides in, cf. http://stackoverflow.com/a/13948634/2559632
    QMetaObject::invokeMethod(_ownFlowWorker, "start");
}

void OwnFlowHandler::pause()
{
    _ownFlowWorker->pause();
}

void OwnFlowHandler::stop()
{
    _ownFlowWorker->pause();
    _ownFlowWorkerThread.quit();
    _ownFlowWorkerThread.wait();
}
