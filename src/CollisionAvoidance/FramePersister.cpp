#include "FramePersister.h"
#include "CollisionAvoidanceSettings.h"
#include "DrawHelper.h"
#include "HeatMap.h"
#include "DrawHelper.h"

#include <QDir>

#include <string>

using namespace std;
using namespace cv;

FramePersister::FramePersister(CollisionAvoidanceSettings& settings, QObject *parent)
    : QObject(parent)
    , _settings(settings)
{
	if(_settings.WriteToOutputEnabled && _settings.ClearOldFramesEnabled)
	{
		if(_settings.WriteRawFrames)
		{
			checkOrMakeDirectory(_settings.RawFramesPath);
			clearDirectory(_settings.RawFramesPath);
		}
		if(_settings.WriteBadFrames)
		{
			checkOrMakeDirectory(_settings.BadFramesPath);
			clearDirectory(_settings.BadFramesPath);
		}
		if(_settings.WriteGoodFrames)
		{
			checkOrMakeDirectory(_settings.GoodFramesPath);
			clearDirectory(_settings.GoodFramesPath);
		}
		if(_settings.WriteHistogramFrames)
		{
			checkOrMakeDirectory(_settings.HistogramFramesPath);
			clearDirectory(_settings.HistogramFramesPath);
		}
		if(_settings.WriteOpticalFlowFrames)
		{
			checkOrMakeDirectory(_settings.OpticalFlowFramesPath);
			clearDirectory(_settings.OpticalFlowFramesPath);
		}
		if(_settings.WriteUiFrames)
		{
			checkOrMakeDirectory(_settings.UiFramesPath);
			clearDirectory(_settings.UiFramesPath);
		}
	}
}

void FramePersister::rawFrameReady(const cv::Mat& rawFrame)
{
	if(!_settings.WriteRawFrames)
		return;

	auto fileName = _settings.RawFramesPath + QString::number(_rawCount++) + ".jpg";
    persistFrame(rawFrame, fileName);
}

void FramePersister::foeFrameReady(const cv::Mat& frame, std::shared_ptr<hw::FocusOfExpansionDto> foeFiltered, std::shared_ptr<hw::FocusOfExpansionDto> foeMeasured, std::shared_ptr<hw::Divergence> divergence)
{
	Q_UNUSED(foeFiltered);
	Q_UNUSED(foeMeasured);
	Q_UNUSED(divergence);

	if(!_settings.WriteGoodFrames)
		return;

    auto fileName = _settings.GoodFramesPath + QString::number(_foeAndBadCount++) + ".jpg";
	persistFrame(frame, fileName);
}

void FramePersister::badFrameReady(const cv::Mat& badFrame, unsigned long long skipFrameCount, unsigned long long totalFrameCount, std::shared_ptr<hw::FocusOfExpansionDto> foeMeasured)
{
	Q_UNUSED(skipFrameCount);
	Q_UNUSED(totalFrameCount);
	Q_UNUSED(foeMeasured);

	if(!_settings.WriteBadFrames)
		return;

	auto fileName = _settings.BadFramesPath + QString::number(_foeAndBadCount++) + ".jpg";
	persistFrame(badFrame, fileName);
}

void FramePersister::opticalFlowReady(const cv::Mat& opticalFlow)
{
	if(!_settings.WriteOpticalFlowFrames)
		return;

	auto fileName = _settings.OpticalFlowFramesPath + QString::number(_opticalFlowCount++) + ".jpg";
	
    cv::Mat flowOverlay(opticalFlow.size(), CV_8UC3, cv::Scalar(0,0,0));;
    DrawHelper::drawOpticalFlowMap(opticalFlow, flowOverlay, cv::Scalar(0, 255, 0)); // GREEN

	persistFrame(flowOverlay, fileName);
}

void FramePersister::histogramReady(const cv::Mat& histogram)
{
	if(!_settings.WriteHistogramFrames)
		return;

	auto fileName = _settings.HistogramFramesPath + QString::number(_hCount++) + ".jpg";
	persistFrame(HeatMap::createHeatMap(histogram), fileName);
}

void FramePersister::uiFrameReady(const cv::Mat& frame)//, std::shared_ptr<hw::FocusOfExpansionDto> foeFiltered, std::shared_ptr<hw::FocusOfExpansionDto> foeMeasured, std::shared_ptr<hw::Divergence> divergence)
{
	if(!_settings.WriteUiFrames)
		return;

	auto fileName = _settings.UiFramesPath + QString::number(_uiCount++) + ".jpg";
	persistFrame(frame, fileName);
}


void FramePersister::clearDirectory(QString& path)
{
	// aso http://stackoverflow.com/a/18409626
	QDir dir(path);
	dir.setNameFilters(QStringList() << "*.jpg");
	dir.setFilter(QDir::Files);
	foreach(QString dirFile, dir.entryList())
	{
	    dir.remove(dirFile);
	}
}

void FramePersister::checkOrMakeDirectory(QString& path)
{
    QDir dir(path);
	if (!dir.exists()) 
	{
		dir.mkpath(".");
	}
}   

void FramePersister::persistFrame(const cv::Mat& frame, QString& path)
{
	if(!_settings.WriteToOutputEnabled)
		return;

    cv::imwrite(path.toStdString(), frame);
}

void FramePersister::persistFrame(const cv::Mat& frame, QString& path, std::vector<std::string>& lines)
{
	Mat tmp = frame;
	if(lines.size()>0)
		tmp = DrawHelper::renderText(frame, lines);

    persistFrame(tmp, path);
}
