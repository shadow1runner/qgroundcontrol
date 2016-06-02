#ifndef COLLISIONAVOIDANCE_COLLISIONAVOIDANCEUNITTEST_H
#define COLLISIONAVOIDANCE_COLLISIONAVOIDANCEUNITTEST_H

#include "UnitTest.h"

class CollisionAvoidanceTestBase : public UnitTest
{
    Q_OBJECT
    
public:
    CollisionAvoidanceTestBase(void);

protected:
    void _init();
    void _cleanup(void);
};

#endif