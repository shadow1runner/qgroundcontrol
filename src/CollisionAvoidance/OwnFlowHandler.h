#ifndef COLLISIONAVOIDANCE_OWNFLOWHANDLER_H
#define COLLISIONAVOIDANCE_OWNFLOWHANDLER_H

#include <string> 

#include <QObject>
#include <QQmlListProperty>
#include <QQuickImageProvider>
#include <QThread>
#include <QDebug>

#include <opencv2/core.hpp>

#include "QGCToolbox.h"
#include "BufferedFrameGrabber.h"
#include "OwnFlow.h"
#include "Converter.h"
#include "FocusOfExpansionDto.h"
#include "CollisionAvoidanceDataProvider.h"
#include "CollisionAvoidanceSettings.h"

class OwnFlowWorker;
class CollisionActor;

class OwnFlowHandler : public QGCTool
{
	Q_OBJECT

public:
    OwnFlowHandler(QGCApplication* app);
    ~OwnFlowHandler();

    void start();
    void pause();
    void stop();
    void reset();
    
    // Overrides from QGCTool
    virtual void setToolbox(QGCToolbox* toolbox);

    OwnFlowWorker* ownFlowWorker();
    CollisionActor* collisionActor();

private:
	CollisionAvoidanceSettings& _settings;

    QThread _ownFlowWorkerThread;
	OwnFlowWorker* _ownFlowWorker = nullptr;

    CollisionActor* _collisionActor = nullptr;
};

#endif // COLLISIONAVOIDANCE_OWNFLOWHANDLER_H
