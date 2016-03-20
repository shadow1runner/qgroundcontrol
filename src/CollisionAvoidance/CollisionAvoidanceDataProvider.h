#ifndef COLLISIONAVOIDANCE_COLLISIONAVOIDANCEDataPROVIDER_H
#define COLLISIONAVOIDANCE_COLLISIONAVOIDANCEDataPROVIDER_H

#include <QObject>
#include <QQmlListProperty>
#include <QQuickImageProvider>

#include <opencv2/core.hpp>

#include "QGCToolbox.h"

class CollisionAvoidanceDataProvider : public QGCTool, public QQuickImageProvider
{
public:
    CollisionAvoidanceDataProvider    (QGCApplication* app);
    ~CollisionAvoidanceDataProvider   ();
    QImage  requestImage              (const QString & id, QSize * size, const QSize & requestedSize);
    void    setImage                  (QImage* pImage, int id = 0);
    void    setToolbox                (QGCToolbox *toolbox);
private:
    QImage  cvMatToQImage(cv::Mat mat);
    void    setFrame(cv::Mat mat);

    QImage _pImage;
};


#endif // COLLISIONAVOIDANCE_COLLISIONAVOIDANCEDataPROVIDER_H
