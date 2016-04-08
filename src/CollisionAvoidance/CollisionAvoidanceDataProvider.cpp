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

#include <opencv2/highgui.hpp>

#include <QPainter>
#include <QFont>

CollisionAvoidanceDataProvider::CollisionAvoidanceDataProvider(QGCApplication *app)
    : QGCTool(app)
    , QQuickImageProvider(QQmlImageProviderBase::Image)
    , _activeVehicle(NULL)
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
   _pImage = QImage(320, 240, QImage::Format_RGBA8888);
   _pImage.fill(Qt::black);
   QPainter painter(&_pImage);
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
    return _pImage;
}

void CollisionAvoidanceDataProvider::_activeVehicleChanged(Vehicle* activeVehicle)
{
  _activeVehicle = activeVehicle; // might be NULL
}

void CollisionAvoidanceDataProvider::foeReady(const cv::Mat& frame, std::shared_ptr<hw::FocusOfExpansionDto> foeFiltered, std::shared_ptr<hw::FocusOfExpansionDto> foeMeasured, std::shared_ptr<hw::Divergence> divergence) {
  auto guiImage = renderGuiImage(frame, foeFiltered, foeMeasured, divergence);

  _pImage = cvMatToQImage(guiImage);

  std::cout << "Foe: " << foeFiltered->getFoE() << std::endl;
  std::cout << "divergence: " << divergence->getDivergence() << std::endl;
  std::cout << "measuredFoe: " << foeMeasured->getFoE() << std::endl;
  std::cout << "inlierProportion: " << foeFiltered->getInlierProportion() << std::endl;
  std::cout << "numberOfInliers: " << foeFiltered->getNumberOfInliers() << std::endl;
  std::cout << "numberOfParticles: " << foeFiltered->getNumberOfParticles() <<std::endl;
  
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
    // http://stackoverflow.com/a/12312326/2559632
    cvtColor(mat, mat, CV_BGR2RGB);
    // cv::Mat tmpMat;
    // resize(mat, tmpMat, cv::Size(0,0), 2, 2, cv::INTER_LINEAR);
    return QImage((uchar*)mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
}

cv::Mat CollisionAvoidanceDataProvider::renderGuiImage(const cv::Mat& frame, std::shared_ptr<hw::FocusOfExpansionDto> foeFiltered, std::shared_ptr<hw::FocusOfExpansionDto> foeMeasured, std::shared_ptr<hw::Divergence> divergence)
{
    Q_UNUSED(frame);
    Q_UNUSED(divergence);

   // ========================== DRAW =====================
   std::vector<cv::Mat> canvas;

   cv::Scalar color = foeFiltered->getInlierProportion() > inlierProportion ? GOOD_CONFIDENCE : BAD_CONFIDENCE;

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
   DrawHelper::drawOpticalFlowMap(opticalFlow, flowOverlay, GREEN);
   DrawHelper::drawRings(flowOverlay, foeMeasured->getFoE(), color);
   DrawHelper::drawRings(flowOverlay, foeFiltered->getFoE(), cv::Scalar(255, 156, 0));
   canvas.push_back(flowOverlay);

//   auto hsv = DrawHelper::visualiceFlowAsHsv(opticalFlow);
//   DrawHelper::drawRings(hsv, foeMeasured->getFoE(), color);
//   DrawHelper::drawRings(hsv, foeFiltered->getFoE(), cv::Scalar(255, 156, 0));
//   canvas.push_back(hsv);

   auto combined = DrawHelper::makeRowCanvas(canvas, cv::Scalar(64, 64, 64));
//   //    cv::imwrite(OUTPUT_DIR + std::to_string(frame_number) + ".jpg", combined);
//   imshow("combined", combined);
//   cv::waitKey(30);
  // ========================== DRAW - END =====================
   return combined;
}    
