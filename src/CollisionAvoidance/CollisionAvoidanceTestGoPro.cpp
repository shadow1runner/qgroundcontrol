#include "CollisionAvoidanceTestGoPro.h"

#include <QDebug>

CollisionAvoidanceTestGoPro::CollisionAvoidanceTestGoPro() 
{

}

void CollisionAvoidanceTestGoPro::init(void)
{
    UnitTest::init();
    _init();
}

void CollisionAvoidanceTestGoPro::cleanup(void)
{
    _cleanup();
}

void CollisionAvoidanceTestGoPro::first_test(void)
{
	qDebug() << "servus";
}
