#ifndef COLLISIONAVOIDANCE_COLLISIONAVOIDANCEDataPROVIDER_H
#define COLLISIONAVOIDANCE_COLLISIONAVOIDANCEDataPROVIDER_H

#include <QObject>
#include <QQmlListProperty>
#include <QQuickImageProvider>

#include <opencv2/core.hpp>

#include "QGCToolbox.h"
#include "AvgWatch.h"

class Vehicle;

class CollisionAvoidanceDataProvider : public QGCTool, public QQuickImageProvider
{
    Q_OBJECT

public:
    CollisionAvoidanceDataProvider    (QGCApplication* app);
    ~CollisionAvoidanceDataProvider();
    QImage  requestImage              (const QString &url, QSize * size, const QSize & requestedSize);
    void    setToolbox                (QGCToolbox *toolbox);

private slots:
    void _activeVehicleChanged  (Vehicle* activeVehicle);
public slots:
    void uiFrameReady(const cv::Mat& frame, unsigned long long frameNumber);
    void rawFrameReady(const cv::Mat& rawImage);

    
private:
    Vehicle* _activeVehicle;
    QImage* _qRawImage = NULL; // raw image
    QImage* _qImage = NULL; // the current displayed image
	AvgWatch _sw;
    
	QImage* cvMatToQImage(const cv::Mat& mat);
};

#endif // COLLISIONAVOIDANCE_COLLISIONAVOIDANCEDataPROVIDER_H
