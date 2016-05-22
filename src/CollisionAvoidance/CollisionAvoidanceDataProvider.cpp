/*=====================================================================

QGroundControl Open Source Ground Control Station

(c) 2009 - 2015 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>

This file is part of the QGROUNDCONTROL project

QGROUNDCONTROL is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

QGROUNDCONTROL is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with QGROUNDCONTROL. If not, see <http://www.gnu.org/licenses/>.

======================================================================*/

/**
* @file
*   @brief Image Provider
*
*   @author Helmut Wolf
*
*/


#include "CollisionAvoidanceDataProvider.h"
#include "MultiVehicleManager.h"
#include "Vehicle.h"
#include "QGCApplication.h"

#include <QPainter>
#include <QFont>

#include <string>
#include <sstream>

CollisionAvoidanceDataProvider::CollisionAvoidanceDataProvider(QGCApplication *app)
    : QGCTool(app)
    , QQuickImageProvider(QQmlImageProviderBase::Image)
    , _activeVehicle(NULL)
    , _settings(CollisionAvoidanceSettings::getInstance())
{
}

CollisionAvoidanceDataProvider::~CollisionAvoidanceDataProvider()
{
}

void CollisionAvoidanceDataProvider::setToolbox(QGCToolbox *toolbox)
{
   QGCTool::setToolbox(toolbox);
   connect(toolbox->multiVehicleManager(), &MultiVehicleManager::activeVehicleChanged, this, &CollisionAvoidanceDataProvider::_activeVehicleChanged);

   //-- Dummy temporary image until something comes along
   _qImage = QImage(320, 240, QImage::Format_RGBA8888);
   _qImage.fill(Qt::black);
   QPainter painter(&_qImage);
   QFont f = painter.font();
   f.setPixelSize(20);
   painter.setFont(f);
   painter.setPen(Qt::white);
   painter.drawText(QRectF(0, 0, 320, 240), Qt::AlignCenter, "Idle");
}

QImage CollisionAvoidanceDataProvider::requestImage(const QString & /* image url with vehicle id*/, QSize *, const QSize &)
{
/*
    The QML side will request an image using a special URL, which we've registered as QGCImages.
    The URL follows this format (or anything you want to make out of it after the "QGCImages" part):

    "image://OwnFlow/vvv/iii"

    Where:
        vvv: Some vehicle id
        iii: An auto incremented index (which forces the Item to reload the image)

    The image index is incremented each time a new image arrives. A signal is emitted and the QML side
    updates its contents automatically.

        Image {
            source:     "image://OwnFlow/" + _activeVehicle.id + "/" + _activeVehicle.flowImageIndex
            width:      parent.width * 0.5
            height:     width * 0.75
            cache:      false
            anchors.centerIn: parent
            fillMode: Image.PreserveAspectFit
        }

    For now, we don't even look at the URL. This will have to be fixed if we're to support multiple
    vehicles transmitting flow images.
*/
    return _qImage;
}

void CollisionAvoidanceDataProvider::_activeVehicleChanged(Vehicle* activeVehicle)
{
  _activeVehicle = activeVehicle; // might be NULL
}

void CollisionAvoidanceDataProvider::foeReady(const cv::Mat& frame, std::shared_ptr<hw::FocusOfExpansionDto> foeFiltered, std::shared_ptr<hw::FocusOfExpansionDto> foeMeasured, std::shared_ptr<hw::Divergence> divergence) {
  Q_UNUSED(divergence);

  _uiMat = renderGoodFrame(frame, foeFiltered, foeMeasured, divergence);
  emit uiFrameReady(_uiMat);
  emit uiFrameDetailsReady(_uiMat, foeFiltered, foeMeasured, divergence);
  _qImage = cvMatToQImage(_uiMat);
  
  if(_activeVehicle!=NULL)
     _activeVehicle->increaseCollisionAvoidanceImageIndex();
}    

void CollisionAvoidanceDataProvider::badFrame(const cv::Mat& badFrame, unsigned long long skipFrameCount, unsigned long long totalFrameCount, std::shared_ptr<hw::FocusOfExpansionDto> foeMeasured)
{
  Q_UNUSED(skipFrameCount);
  Q_UNUSED(totalFrameCount);

  if(!_settings.DisplayBadFramesInUi)
    return;

  _uiMat = renderBadFrame(badFrame, foeMeasured);
  emit uiFrameReady(_uiMat);
  _qImage = cvMatToQImage(_uiMat);
  
  if(_activeVehicle!=NULL)
     _activeVehicle->increaseCollisionAvoidanceImageIndex();
}

void CollisionAvoidanceDataProvider::opticalFlowReady(const cv::Mat& opticalFlow)
{
  this->opticalFlow = opticalFlow;
}

void CollisionAvoidanceDataProvider::histogramReady(const cv::Mat& histogram)
{
  heatMap = HeatMap::createHeatMap(histogram);
}

QImage CollisionAvoidanceDataProvider::cvMatToQImage(const cv::Mat& mat) {
    ++_frameCount;
    cv::Mat tmp;
    // http://stackoverflow.com/a/12312326/2559632
    cvtColor(mat, tmp, CV_BGR2RGB);
    // cv::Mat tmpMat;
    // resize(mat, tmpMat, cv::Size(0,0), 2, 2, cv::INTER_LINEAR);
    return QImage((uchar*)tmp.data, tmp.cols, tmp.rows, tmp.step, QImage::Format_RGB888);
}

cv::Mat CollisionAvoidanceDataProvider::renderGoodFrame(
  const cv::Mat& frame,
  std::shared_ptr<hw::FocusOfExpansionDto> foeFiltered,
  std::shared_ptr<hw::FocusOfExpansionDto> foeMeasured,
  std::shared_ptr<hw::Divergence> divergence)
{
   std::vector<cv::Mat> canvas;

   cv::Scalar color = GOOD_FRAME_COLOR;

   DrawHelper::drawRings(heatMap, foeMeasured->getFoE(), color);
   DrawHelper::drawRings(heatMap, foeFiltered->getFoE(), cv::Scalar(255, 156, 0));
   canvas.push_back(heatMap);

   cv::Mat bgr;
   cv::cvtColor(frame, bgr, cv::COLOR_GRAY2BGR);
   DrawHelper::drawRings(bgr, foeMeasured->getFoE(), color);
   DrawHelper::drawRings(bgr, foeFiltered->getFoE(), cv::Scalar(255, 156, 0));
   canvas.push_back(bgr);

   cv::Mat flowOverlay;
   cv::cvtColor(frame, flowOverlay, cv::COLOR_GRAY2BGR);
   DrawHelper::drawOpticalFlowMap(opticalFlow, flowOverlay, GREEN, 16, _settings.OpticalFlowVectorVisualizationFactor, _settings.OpticalFlowVectorVisualizationFactor);
   DrawHelper::drawRings(flowOverlay, foeMeasured->getFoE(), color);
   DrawHelper::drawRings(flowOverlay, foeFiltered->getFoE(), cv::Scalar(255, 156, 0));
   canvas.push_back(flowOverlay);

   auto hsv = DrawHelper::visualiceFlowAsHsv(opticalFlow);
   DrawHelper::drawRings(hsv, foeMeasured->getFoE(), color);
   DrawHelper::drawRings(hsv, foeFiltered->getFoE(), cv::Scalar(255, 156, 0));
   canvas.push_back(hsv);

   auto combined = DrawHelper::makeRowCanvas(canvas, cv::Scalar(64, 64, 64));

   std::vector<std::string> lines;
   std::stringstream ss;
   ss << "#" << _frameCount << ": GOOD; " << std::setprecision(4) << divergence->getDivergence(); lines.push_back(ss.str()); ss.str(std::string());
   ss << "FoE: (" << foeFiltered->getFoE().x << ", " << foeFiltered->getFoE().y << ") @ " <<  std::setprecision(3) << foeFiltered->getInlierProportion()*100 << "%"; lines.push_back(ss.str()); ss.str(std::string());
   ss << "(foe: (" << foeMeasured->getFoE().x << ", " << foeMeasured->getFoE().y << ") @ "  << std::setprecision(3) << foeMeasured->getInlierProportion()*100 << "%)"; lines.push_back(ss.str()); ss.str(std::string());

   return DrawHelper::renderText(combined, lines);
}  

cv::Mat CollisionAvoidanceDataProvider::renderBadFrame(
  const cv::Mat& frame,
  std::shared_ptr<hw::FocusOfExpansionDto> foeMeasured)
{
   std::vector<cv::Mat> canvas;
   cv::Scalar color = BAD_FRAME_COLOR;

   DrawHelper::drawRings(heatMap, foeMeasured->getFoE(), color);
   canvas.push_back(heatMap);

   cv::Mat bgr;
   cv::cvtColor(frame, bgr, cv::COLOR_GRAY2BGR);
   DrawHelper::drawRings(bgr, foeMeasured->getFoE(), color);
   canvas.push_back(bgr);

   cv::Mat flowOverlay;
   cv::cvtColor(frame, flowOverlay, cv::COLOR_GRAY2BGR);
   DrawHelper::drawOpticalFlowMap(opticalFlow, flowOverlay, GREEN);
   DrawHelper::drawRings(flowOverlay, foeMeasured->getFoE(), color);
   canvas.push_back(flowOverlay);

   auto hsv = DrawHelper::visualiceFlowAsHsv(opticalFlow);
   DrawHelper::drawRings(hsv, foeMeasured->getFoE(), color);
   canvas.push_back(hsv);

   auto combined = DrawHelper::makeRowCanvas(canvas, cv::Scalar(64, 64, 64));

   std::vector<std::string> lines;
   std::stringstream ss;
   ss << "#" << _frameCount << ": BAD"; lines.push_back(ss.str()); ss.str(std::string());
   ss << "No FoE"; lines.push_back(ss.str()); ss.str(std::string());
   ss << "(foe: (" << foeMeasured->getFoE().x << ", " << foeMeasured->getFoE().y << ") - "  << std::setprecision(3) << foeMeasured->getInlierProportion()*100 << "%)"; lines.push_back(ss.str()); ss.str(std::string());

   return DrawHelper::renderText(combined, lines);
}
