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

class OwnFlowHandler : public QGCTool
{
public:
    OwnFlowHandler    				  (QGCApplication* app);
    ~OwnFlowHandler   				  ();

    void start();
    void stop();
    
    // Overrides from QGCTool
    virtual void setToolbox(QGCToolbox* toolbox);

private:
	void loadSettings();
	void storeSettings();

	// input file to determine FOE from
	std::string FILENAME;
	// output file where the individual canvases are persisted, make sure it exists, use procmon
	std::string OUTPUT_DIR;
	// scale factor for each frame; n = 1 --> half resolution n = 2 --> 1/4 resolution
	int SUBSAMPLE_AMOUNT;
	// should be divisible by the amount of cores, the reminder will be silently ignored...
	int PARTICLES;
	// 1 -> maximum element, 2 -> 4 neighborhood to the lower right; 3 -> 9 neighborhood etc
	int WINDOW_SIZE;
	// any frame with a lower inlier proportion will be discarded
	int INLIER_PROPORTION_THRESHOLD;
	// patch size used for determining the divergence
	int DIVERGENCE_PATCHSIZE;
	double DIVERGENCE_THRESHOLD;

    hw::BufferedFrameGrabber * frame_grabber;
    
    QThread ownFlowThread;
    hw::OwnFlow * ownFlow;
    
    QThread converterThread;
    hw::Converter * converter;

    CollisionAvoidanceDataProvider * _collisionAvoidanceDataProvider;
};


#endif // COLLISIONAVOIDANCE_OWNFLOWHANDLER_H
