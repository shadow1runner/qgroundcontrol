#ifndef COLLISIONAVOIDANCE_COLLISIONAVOIDANCETESTBOSCAMCHEAPFISHEYE_H
#define COLLISIONAVOIDANCE_COLLISIONAVOIDANCETESTBOSCAMCHEAPFISHEYE_H

#include "CollisionAvoidanceTestBase.h"

class CollisionAvoidanceTestBoscamCheapFisheye : public CollisionAvoidanceTestBase
{
    Q_OBJECT
		
public:
    CollisionAvoidanceTestBoscamCheapFisheye();

private slots:
    void init();
    void cleanup(); 

    // FPV videos
    void fenceCrash();
    void fenceCrash2();
    void roofThinObstacle();
    void groundRaisesCa();
    void groundHoveringNoCa();
    void _5mHoveringNoCa();
    void fastFenceFlyOver();
    void fastMetalFence();
};

#endif
