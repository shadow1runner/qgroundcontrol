#ifndef COLLISIONAVOIDANCE_COLLISIONAVOIDANCEDataPROVIDER_H
#define COLLISIONAVOIDANCE_COLLISIONAVOIDANCEDataPROVIDER_H

#include <vector>
#include <memory>
#include <iostream>

#include <QObject>
#include <QQmlListProperty>
#include <QQuickImageProvider>

#include <opencv2/core.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include "QGCToolbox.h"
#include "DrawHelper.h"
#include "HeatMap.h"
#include "OwnFlow.h"
#include "FocusOfExpansionDto.h"

class CollisionAvoidanceDataProvider : public QGCTool, public QQuickImageProvider
{
public:
    CollisionAvoidanceDataProvider    (QGCApplication* app);
    ~CollisionAvoidanceDataProvider   ();
    QImage  requestImage              (const QString & id, QSize * size, const QSize & requestedSize);
    void    setImage                  (QImage* pImage, int id = 0);
    void    setToolbox                (QGCToolbox *toolbox);

public slots:
    void foeReady(const cv::Mat& frame, std::shared_ptr<hw::FocusOfExpansionDto> foeFiltered, std::shared_ptr<hw::FocusOfExpansionDto> foeMeasured, std::shared_ptr<hw::Divergence> divergence);

    void opticalFlowReady(const cv::Mat& opticalFlow);

    void histogramReady(const cv::Mat& histogram);

private:
    QImage  cvMatToQImage(cv::Mat mat);
    cv::Mat draw();

    QImage _pImage;
    std::shared_ptr<hw::FocusOfExpansionDto> foeFiltered;
    std::shared_ptr<hw::FocusOfExpansionDto> foeMeasured;
    std::shared_ptr<hw::Divergence> divergence;

    double inlierProportion;
    cv::Mat opticalFlow;
    cv::Mat heatMap;
    const cv::Scalar GREEN = cv::Scalar(0, 255, 0);
    const cv::Scalar GOOD_CONFIDENCE = cv::Scalar(255, 0, 0);
    const cv::Scalar BAD_CONFIDENCE = cv::Scalar(0, 255, 0);
    
};

#endif // COLLISIONAVOIDANCE_COLLISIONAVOIDANCEDataPROVIDER_H
