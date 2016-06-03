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
    Q_INVOKABLE void pause();
    void stop();
    void reset();
    
    // Overrides from QGCTool
    virtual void setToolbox(QGCToolbox* toolbox);

    OwnFlowWorker* ownFlowWorker();
    CollisionActor* collisionActor();

private slots:
    void _collisionImmanent(const cv::Mat& frame, unsigned long long frameNumber, std::shared_ptr<cv::Point2i> foeFiltered, std::shared_ptr<hw::FocusOfExpansionDto> foe, const hw::CollisionLevel collisionLevel, double lastDivergence, double avgDivergence);

private:
	CollisionAvoidanceSettings& _settings;

    QThread _ownFlowWorkerThread;
	OwnFlowWorker* _ownFlowWorker = nullptr;

    CollisionActor* _collisionActor = nullptr;
};

#endif // COLLISIONAVOIDANCE_OWNFLOWHANDLER_H
