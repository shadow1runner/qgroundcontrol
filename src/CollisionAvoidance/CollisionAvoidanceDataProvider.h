#ifndef COLLISIONAVOIDANCE_COLLISIONAVOIDANCEDataPROVIDER_H
#define COLLISIONAVOIDANCE_COLLISIONAVOIDANCEDataPROVIDER_H

#include <QObject>
#include <QQmlListProperty>
#include <QQuickImageProvider>

#include "QGCToolbox.h"
#include "AvgWatch.h"

class Vehicle;

class CollisionAvoidanceDataProvider : public QGCTool, public QQuickImageProvider
{
    Q_OBJECT

public:
    CollisionAvoidanceDataProvider    (QGCApplication* app);
    QImage  requestImage              (const QString & id, QSize * size, const QSize & requestedSize);
    void    setToolbox                (QGCToolbox *toolbox);

private slots:
    void _activeVehicleChanged  (Vehicle* activeVehicle);
public slots:
    void qtUiFrameReady(const QImage& frame);    
    
private:
    Vehicle* _activeVehicle;
    QImage _qImage; // the current displayed image 
	AvgWatch _sw;
    
};

#endif // COLLISIONAVOIDANCE_COLLISIONAVOIDANCEDataPROVIDER_H
