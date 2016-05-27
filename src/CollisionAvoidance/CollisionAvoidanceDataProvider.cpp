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
#include <QDateTime>

#include <string>
#include <sstream>

CollisionAvoidanceDataProvider::CollisionAvoidanceDataProvider(QGCApplication *app)
  : QGCTool(app)
  , QQuickImageProvider(QQmlImageProviderBase::Image)
  , _activeVehicle(NULL)
  , _qImage(320, 240, QImage::Format_RGBA8888)
  , _sw("QML Refresh Rate")
{
    //-- Dummy temporary image until something comes along
  _qImage.fill(Qt::black);
  QPainter painter(&_qImage);
  QFont f = painter.font();
  f.setPixelSize(20);
  painter.setFont(f);
  painter.setPen(Qt::white);
  painter.drawText(QRectF(0, 0, 320, 240), Qt::AlignCenter, "Idle");
}

void CollisionAvoidanceDataProvider::setToolbox(QGCToolbox *toolbox)
{
  QGCTool::setToolbox(toolbox);
  connect(toolbox->multiVehicleManager(), &MultiVehicleManager::activeVehicleChanged, this, &CollisionAvoidanceDataProvider::_activeVehicleChanged);
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
  if(_sw.isRunning()) {
    _sw.stopTick();
    qDebug() << "DP" << QString::fromStdString(_sw.prettyFormat()); 
   _sw.startTick();
  }
  return _qImage;
}

void CollisionAvoidanceDataProvider::_activeVehicleChanged(Vehicle* activeVehicle)
{
  _activeVehicle = activeVehicle; // might be NULL
}

void CollisionAvoidanceDataProvider::qtUiFrameReady(const QImage& qImage)
{
  static int count = 0;
  qDebug() << "DP: # of UI frames ready: " << ++count;
  if(_activeVehicle!=NULL) {
    _qImage = qImage;
    _activeVehicle->increaseCollisionAvoidanceImageIndex();
  }
}
