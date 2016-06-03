#ifndef COLLISIONAVOIDANCE_COLLISIONAVOIDANCEUNITTEST_H
#define COLLISIONAVOIDANCE_COLLISIONAVOIDANCEUNITTEST_H

#include "UnitTest.h"
#include "CollisionAvoidanceSettings.h"

class QGCToolbox;
class CollisionAvoidanceTestSettingsDto;


class CollisionAvoidanceTestBase : public UnitTest
{
    Q_OBJECT
    
public:
    CollisionAvoidanceTestBase(void);

protected:
    void _init();
    void _cleanup(void);
    void _testCa(CollisionAvoidanceSettings& settings, QGCToolbox* toolbox, CollisionAvoidanceTestSettingsDto& dto);
};

struct CollisionAvoidanceTestSettingsDto
{
	bool shouldTriggerCollisionImmanent = true;
	int lowerFrameNumberBound;
	int upperFrameNumberBound;
};

#endif