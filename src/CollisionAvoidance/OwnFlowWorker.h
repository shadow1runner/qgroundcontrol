#ifndef OWNFLOWWORKER_H
#define OWNFLOWWORKER_H

#include <string>

#include <QObject>
#include <QThread>

#include "CollisionAvoidanceSettings.h"
#include "CollisionAvoidanceDataProvider.h"
#include "Converter.h"
#include "OwnFlow.h"
#include "QGCToolbox.h"
#include "BufferedFrameGrabber.h"
#include "FocusOfExpansionDto.h"
#include "CollisionAvoidanceDataProvider.h"

class OwnFlowHandler;

class OwnFlowWorker : public QObject {
	Q_OBJECT

public:
    OwnFlowWorker(const CollisionAvoidanceSettings& settings, const CollisionAvoidanceDataProvider* const collisionAvoidanceDataProvider);
    ~OwnFlowWorker();

    hw::OwnFlow* ownFlow();

public slots:
	void start();
	void stop();

private:
    const CollisionAvoidanceSettings& _settings;
    const CollisionAvoidanceDataProvider * const _collisionAvoidanceDataProvider = nullptr;

    hw::OwnFlow _ownFlow;
	hw::Converter _converter;

    QThread _ownFlowThread;
    QThread _converterThread;
};

#endif // OWNFLOWWORKER_H
