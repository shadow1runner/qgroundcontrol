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

    void go2();	
    void go3();	
    void go4();
    void go5();
    void go6();
};

#endif
