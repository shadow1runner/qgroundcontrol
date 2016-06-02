#ifndef COLLISIONAVOIDANCETESTGOPRO_H
#define COLLISIONAVOIDANCETESTGOPRO_H

#include "CollisionAvoidanceTestBase.h"

class CollisionAvoidanceTestGoPro : public CollisionAvoidanceTestBase
{
    Q_OBJECT
		
public:
    CollisionAvoidanceTestGoPro();

private slots:
    void init(void);
    void cleanup(void);

    void first_test(void);

	
};

#endif
