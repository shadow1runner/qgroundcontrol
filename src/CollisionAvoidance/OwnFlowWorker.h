#ifndef OWNFLOWWORKER_H
#define OWNFLOWWORKER_H

#include <string>
#include <memory>
#include <opencv2/core.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include <QObject>
#include <QThread>

#include "QGCToolbox.h"
#include "CollisionAvoidanceSettings.h"
#include "CollisionAvoidanceDataProvider.h"
#include "Converter.h"
#include "OwnFlow.h"
#include "BufferedFrameGrabber.h"
#include "FocusOfExpansionDto.h"
#include "CollisionAvoidanceDataProvider.h"
#include "FocusOfExpansionDto.h"
#include "Divergence.h"
#include "AvgWatch.h"
#include "OwnFlowGrapher.h"
#include "FramePersister.h"
#include "UiFramePreparer.h"
#include "RoiBuilder.h"

class QGCToolbox;

class OwnFlowWorker : public QObject {
	Q_OBJECT

public:
    OwnFlowWorker(CollisionAvoidanceSettings& settings, QGCToolbox* toolbox);
    ~OwnFlowWorker();

    Q_INVOKABLE void reset();

    hw::OwnFlow* ownFlow();
    OwnFlowGrapher* ownFlowGrapher();
    hw::BufferedFrameGrabber* bufferedFrameGrabber();

public slots:
    void start();
    void pause();
    void stop();

signals:
    void isPausedChanged(bool isPaused);

private:
    CollisionAvoidanceSettings& _settings;
    const CollisionAvoidanceDataProvider * const _collisionAvoidanceDataProvider = nullptr;

    bool _isPaused;

    hw::BufferedFrameGrabber* _frameGrabber = nullptr;

    hw::UiFramePreparer _uiFramePreparer;
    hw::Converter _converter;
    RoiBuilder _roiBuilder;
    hw::OwnFlow _ownFlow;
    OwnFlowGrapher _grapher;
    FramePersister _framePersister;

    QThread _ownFlowThread;
    QThread _converterThread;
    QThread _grapherThread;
    QThread _ioThread;
    QThread _uiFramePreparerThread;
    QThread _roiBuilderThread;
};

#endif // OWNFLOWWORKER_H
