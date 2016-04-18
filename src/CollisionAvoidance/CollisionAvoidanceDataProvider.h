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
#include "CollisionAvoidanceSettings.h"

class Vehicle;
class CollisionAvoidanceSettings;

class CollisionAvoidanceDataProvider : public QGCTool, public QQuickImageProvider
{
    Q_OBJECT

public:
    CollisionAvoidanceDataProvider    (QGCApplication* app);
    ~CollisionAvoidanceDataProvider   ();
    QImage  requestImage              (const QString & id, QSize * size, const QSize & requestedSize);
    void    setToolbox                (QGCToolbox *toolbox);

signals:
    void uiFrameReady(const cv::Mat& frame);    

public slots:
    void foeReady(const cv::Mat& frame, std::shared_ptr<hw::FocusOfExpansionDto> foeFiltered, std::shared_ptr<hw::FocusOfExpansionDto> foeMeasured, std::shared_ptr<hw::Divergence> divergence);

    void badFrame(const cv::Mat& badFrame, unsigned long long skipFrameCount, unsigned long long totalFrameCount, std::shared_ptr<hw::FocusOfExpansionDto> foeMeasured);

    void opticalFlowReady(const cv::Mat& opticalFlow);

    void histogramReady(const cv::Mat& histogram);

private slots:
    void _activeVehicleChanged  (Vehicle* activeVehicle);

private:
    unsigned long long _frameCount=0;

    Vehicle* _activeVehicle;
    CollisionAvoidanceSettings& _settings;

    QImage  cvMatToQImage(const cv::Mat& mat);
    cv::Mat renderGoodFrame(const cv::Mat& frame, std::shared_ptr<hw::FocusOfExpansionDto> foeFiltered, std::shared_ptr<hw::FocusOfExpansionDto> foeMeasured);
    cv::Mat renderBadFrame(const cv::Mat& badFrame, std::shared_ptr<hw::FocusOfExpansionDto> foeMeasured);
    void saveCurrentImageToFile(bool isBadFrame=false);
    void saveRawFrameToFile(const cv::Mat& frame);


    cv::Mat _uiMat; // the current displayed image on the ui as cv::Mat
    QImage _qImage; // .. and as qImage
    std::shared_ptr<hw::FocusOfExpansionDto> foeFiltered;
    std::shared_ptr<hw::FocusOfExpansionDto> foeMeasured;
    std::shared_ptr<hw::Divergence> divergence;

    cv::Mat opticalFlow;
    cv::Mat heatMap;
    const cv::Scalar GREEN = cv::Scalar(0, 255, 0);
    const cv::Scalar GOOD_FRAME_COLOR = cv::Scalar(255, 0, 0);
    const cv::Scalar BAD_FRAME_COLOR = cv::Scalar(0, 255, 0);
    
};

#endif // COLLISIONAVOIDANCE_COLLISIONAVOIDANCEDataPROVIDER_H
