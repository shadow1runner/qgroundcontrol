#ifndef COLLISIONAVOIDANCETESTGOPRO_H
#define COLLISIONAVOIDANCETESTGOPRO_H

#include "CollisionAvoidanceTestBase.h"

class CollisionAvoidanceTestGoPro : public CollisionAvoidanceTestBase
{
    Q_OBJECT
		
public:
    CollisionAvoidanceTestGoPro();

private slots:
    void init();
    void cleanup();

    // sebastian's videos
    void go2();
    void go3();
    void go4();
    void go5();
    void go6();
    void go7();

    // /home/helli/d/m/qgroundcontrol/src/CollisionAvoidance/opticalflow/res/gopro/simulated
    void gopr0109();

    // /home/helli/d/m/qgroundcontrol/src/CollisionAvoidance/opticalflow/res/gopro/air
    void gopr0121();
    void gopr0122();
    void gopr0124();
    void gopr0125();
    void gopr0131();
    void gopr0133();
};

#endif
