#include "OwnFlowHandler.h"
#include "OwnFlowWorker.h"

#include <QThread>

#include "QGCToolbox.h"
#include "QtHelper.h"

OwnFlowHandler::OwnFlowHandler(QGCApplication* app)
    : QGCTool(app)
{
    QtHelper::registerMetaTypes();
}

OwnFlowHandler::~OwnFlowHandler()
{
	stop();

    delete _ownFlowWorker;
}

void OwnFlowHandler::setToolbox(QGCToolbox* toolbox)
{
	QGCTool::setToolbox(toolbox);
    auto collisionAvoidanceDataProvider = toolbox->collisionAvoidanceDataProvider();

    _ownFlowWorker = new OwnFlowWorker(_settings, collisionAvoidanceDataProvider);
    _ownFlowWorker->moveToThread(&_ownFlowWorkerThread);
    connect(&_ownFlowWorkerThread, &QThread::started,
             _ownFlowWorker, &OwnFlowWorker::start);
}

OwnFlowWorker* OwnFlowHandler::ownFlowWorker() 
{
    return _ownFlowWorker;
}


void OwnFlowHandler::start()
{
    qDebug() << "Starting OwnFlowHandler on Thread " << QThread::currentThreadId();
    _ownFlowWorkerThread.start();
}

void OwnFlowHandler::stop()
{
    qDebug() << "Stopping OwnFlowHandler on Thread " << QThread::currentThreadId();
    _ownFlowWorkerThread.quit();
    _ownFlowWorkerThread.wait();
}
