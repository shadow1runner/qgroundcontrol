#ifndef COLLISIONAVOIDANCE_COLLISIONAVOIDANCETESTBOSCAMSYNTHETICFISHEYE_H
#define COLLISIONAVOIDANCE_COLLISIONAVOIDANCETESTBOSCAMSYNTHETICFISHEYE_H

#include "CollisionAvoidanceTestBase.h"

class CollisionAvoidanceTestBoscamSyntheticFisheye : public CollisionAvoidanceTestBase
{
    Q_OBJECT
		
public:
    CollisionAvoidanceTestBoscamSyntheticFisheye();

private slots:
    void init();
    void cleanup(); 
};

#endif
