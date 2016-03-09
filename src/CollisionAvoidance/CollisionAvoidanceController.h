

#ifndef COLLISIONAVOIDANCE_COLLISIONAVOIDANCECONTROLLER_H
#define COLLISIONAVOIDANCE_COLLISIONAVOIDANCECONTROLLER_H

#include <QObject>

#include "UASInterface.h"
#include "AutoPilotPlugin.h"
#include "FactPanelController.h"

class CollisionAvoidanceController : public FactPanelController
{
    Q_OBJECT

public:
    CollisionAvoidanceController(void);

//    Q_PROPERTY(QString customQmlFile MEMBER _customQmlFile NOTIFY customQmlFileChanged)

//    Q_INVOKABLE void sendCommand    (int commandId, QVariant componentId, QVariant confirm, QVariant param1, QVariant param2, QVariant param3, QVariant param4, QVariant param5, QVariant param6, QVariant param7);
//    Q_INVOKABLE void selectQmlFile  (void);
//    Q_INVOKABLE void clearQmlFile   (void);

signals:
//    void customQmlFileChanged   (const QString& customQmlFile);

private slots:
    void _activeVehicleChanged  (Vehicle* activeVehicle);

private:
    UASInterface*       _uas;
//    QString             _customQmlFile;
//    static const char*  _settingsKey;
};

#endif // COLLISIONAVOIDANCE_COLLISIONAVOIDANCECONTROLLER_H
