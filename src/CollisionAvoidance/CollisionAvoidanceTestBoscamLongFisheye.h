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
    void balconyCrash();

//     // FPV videos, 45 deg down
//     void _45deg_5mHover();
//     void _45deg_fastFenceOverfly();
//     void _45deg_fenceCrash2();
    void _45deg_fenceCrash3();
//     void _45deg_fenceCrash();
//     void _45deg_houseHover2();
//     void _45deg_houseHover();
//     void _45deg_noCaHover();
//     void _45deg_slowFenceOverfly();
 };

#endif
