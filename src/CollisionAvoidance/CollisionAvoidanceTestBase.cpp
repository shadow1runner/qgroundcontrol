#include "CollisionAvoidanceTestBase.h"
#include "LinkManager.h"
#ifdef QT_DEBUG
#include "MockLink.h"
#endif
#include "MultiVehicleManager.h"
#include "QGCApplication.h"


CollisionAvoidanceTestBase::CollisionAvoidanceTestBase()
{

}

void CollisionAvoidanceTestBase::_init()
{
	_connectMockLink(MAV_AUTOPILOT_GENERIC);
}

void CollisionAvoidanceTestBase::_cleanup(void)
{
    UnitTest::cleanup();
}