#ifndef COLLISIONAVOIDANCETESTBoscamNormalLense_H
#define COLLISIONAVOIDANCETESTBoscamNormalLense_H

#include "CollisionAvoidanceTestBase.h"

class CollisionAvoidanceTestBoscamNormalLense : public CollisionAvoidanceTestBase
{
    Q_OBJECT
		
public:
    CollisionAvoidanceTestBoscamNormalLense();

private slots:
    void init();
    void cleanup();    

    void bottomCrash();
    void fenceCrash();
};

#endif
