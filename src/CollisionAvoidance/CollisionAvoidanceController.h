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
    Q_INVOKABLE void onStartStopClicked              (bool start);
    
    bool hasCollisionAvoidanceStream() { return _hasCollisionAvoidanceStream; }

    void hasCollisionAvoidanceStreamChanged();

signals:
   void hasCollisionAvoidanceStreamChanged (const bool newValue);

private:
    bool 				_hasCollisionAvoidanceStream;
    OwnFlowHandler*     _ownFlowHandler;
//    static const char*  _settingsKey;
};

#endif // COLLISIONAVOIDANCE_COLLISIONAVOIDANCECONTROLLER_H
