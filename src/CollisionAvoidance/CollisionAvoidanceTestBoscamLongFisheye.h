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
    void balcony_fast_back();
    void balcony_fast_front();
    void balcony_fast_left();
    void balcony_fast_right();
    void balcony_slow_back2();
    void balcony_slow_back();
    void balcony_slow_front2();
    void balcony_slow_front();
    void balcony_slow_left2();
    void balcony_slow_left();
    void balcony_slow_right2();
    void balcony_slow_right();
    void climb();
    void land();


    // FPV videos, 45 deg down
    void _45deg_5mHover(); // veeeeery bad one
    void _45deg_fenceCrash3();
    void _45deg_fenceCrash();
    void _45deg_houseHover2();
    void _45deg_houseHover();
    void _45deg_noCaHover();
    void _45deg_slowFenceOverfly();
    void _45deg_balcony_slow_front();
    void _45deg_balcony_slow_left();
 };

#endif
