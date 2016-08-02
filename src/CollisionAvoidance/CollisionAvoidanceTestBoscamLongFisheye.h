#ifndef COLLISIONAVOIDANCE_COLLISIONAVOIDANCETESTBOSCAMLONGFISHEYE_H
#define COLLISIONAVOIDANCE_COLLISIONAVOIDANCETESTBOSCAMLONGFISHEYE_H

#include "CollisionAvoidanceTestBase.h"

class CollisionAvoidanceTestBoscamLongFisheye : public CollisionAvoidanceTestBase
{
    Q_OBJECT
		
public:
    CollisionAvoidanceTestBoscamLongFisheye();

private slots:
    void init();
    void cleanup(); 

    // FPV videos
    void fenceCrash();
};

#endif
