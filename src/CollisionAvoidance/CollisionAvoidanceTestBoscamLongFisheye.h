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

    // FPV videos, 90 deg down
    void _5mHover();
    void fastFenceOverfly();
    void fenceCrash2();
    void fenceCrash();
    void fenceHover2();
    void fenceHover();
    void houseCrash();
    void balconyCrash(); // works well, all the others above are not that well :S

    // FPV videos, 45 deg down
    void _45deg_5mHover(); // veeeeery bad one
    void _45deg_fenceCrash3();
    void _45deg_fenceCrash();
    void _45deg_houseHover2();
    void _45deg_houseHover();
    void _45deg_noCaHover();
    void _45deg_slowFenceOverfly();
 };

#endif
