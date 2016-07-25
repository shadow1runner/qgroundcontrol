import QtQuick                      2.4
import QtQuick.Controls             1.3

import QGroundControl               1.0
import QGroundControl.FlightMap     1.0
import QGroundControl.ScreenTools   1.0
import QGroundControl.Controls      1.0
import QGroundControl.Palette       1.0
import QGroundControl.Vehicle       1.0
import QGroundControl.Controllers   1.0


Item {
    id: root
    property var _activeVehicle:  QGroundControl.multiVehicleManager.activeVehicle
    QGCPalette { id: qgcPal; colorGroupEnabled: panel.enabled }
    
    QGCViewPanel {
        id:             panel
        anchors.fill:   parent


        Rectangle {
            color: Qt.rgba(0,0,0,0.75)
            anchors.fill:   parent
            
            Flow {
                id: backGround
                anchors.margins: 4
                spacing: 10
                anchors.fill:   parent

                Image {
                    id:         raw
                    source:     _activeVehicle ? "image://OwnFlow/raw/" + _activeVehicle.id + "/" + _activeVehicle.collisionAvoidanceRawImageIndex  : ""
                    width:      parent.width * 0.33
                    height:     width * 0.75
                    cache:      false
                    anchors.left: parent.left
                    anchors.leftMargin: 22
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.rightMargin: 30;
                    fillMode: Image.PreserveAspectFit
                }

                Image {
                    source:     _activeVehicle ? "image://OwnFlow/" + _activeVehicle.id + "/" + _activeVehicle.collisionAvoidanceImageIndex  : ""
                    width:      parent.width * 0.5
                    height:     width * 0.75
                    cache:      false
                    anchors.top: raw.top
                    anchors.topMargin: 7
                    anchors.left: raw.right
                    anchors.leftMargin: 30;
                    anchors.rightMargin: 30;
                    fillMode: Image.PreserveAspectFit
                }
            }
        }
    }
}
