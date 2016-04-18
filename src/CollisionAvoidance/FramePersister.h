#ifndef FRAMEPERSISTER_H
#define FRAMEPERSISTER_H

#include <opencv2/core.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include <memory>

#include <QObject>

#include "FocusOfExpansionDto.h"
#include "Divergence.h"

class CollisionAvoidanceSettings;

class FramePersister : public QObject
{
    Q_OBJECT
public:
    explicit FramePersister(CollisionAvoidanceSettings& settings, QObject *parent = 0);

public slots:
	void rawFrameReady(const cv::Mat& rawFrame);

    void foeFrameReady(const cv::Mat& frame, std::shared_ptr<hw::FocusOfExpansionDto> foeFiltered, std::shared_ptr<hw::FocusOfExpansionDto> foeMeasured, std::shared_ptr<hw::Divergence> divergence);

    void badFrameReady(const cv::Mat& badFrame, unsigned long long skipFrameCount, unsigned long long totalFrameCount, std::shared_ptr<hw::FocusOfExpansionDto> foeMeasured);

    void opticalFlowReady(const cv::Mat& opticalFlow);

    void histogramReady(const cv::Mat& histogram);

    void uiFrameReady(const cv::Mat& ui);

private:
	CollisionAvoidanceSettings& _settings;

    unsigned long long _rawCount=0;    
    unsigned long long _foeAndBadCount=0;    
    unsigned long long _opticalFlowCount=0;
    unsigned long long _hCount=0;  
    unsigned long long _uiCount=0;  

    void clearDirectory(QString& path);
    void checkOrMakeDirectory(QString& path);
    void persistFrame(const cv::Mat& frame, QString& fileName);
};

#endif // FRAMEPERSISTER_H
