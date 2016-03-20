#ifndef COLLISIONAVOIDANCE_COLLISIONAVOIDANCECONTROLLER_H
#define COLLISIONAVOIDANCE_COLLISIONAVOIDANCECONTROLLER_H

#include <QObject>

#include "UASInterface.h"
#include "AutoPilotPlugin.h"
#include "FactPanelController.h"

class CollisionAvoidanceController : public QObject
{
    Q_OBJECT

public:
    CollisionAvoidanceController(QObject* parent = NULL);

    Q_PROPERTY(bool hasCollisionAvoidanceStream       READ hasCollisionAvoidanceStream NOTIFY hasCollisionAvoidanceStreamChanged);
    Q_INVOKABLE void setHasCollisionAvoidanceStream  (bool);
    
    bool hasCollisionAvoidanceStream() { return _hasCollisionAvoidanceStream; }

    void hasCollisionAvoidanceStreamChanged();

signals:
   void hasCollisionAvoidanceStreamChanged (const bool newValue);

private slots:
    void _activeVehicleChanged  (Vehicle* activeVehicle);

private:
    UASInterface*       _uas;
    bool 				_hasCollisionAvoidanceStream;
//    static const char*  _settingsKey;
};

#endif // COLLISIONAVOIDANCE_COLLISIONAVOIDANCECONTROLLER_H
