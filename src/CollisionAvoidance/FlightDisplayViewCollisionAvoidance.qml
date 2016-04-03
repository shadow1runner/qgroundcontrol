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
    
//     Rectangle {
//         id:             noVideo
//         anchors.fill:   parent
//         color:          Qt.rgba(0,0,0,0.75)
// //        visible:        !_controller.hasCollisionAvoidanceStream
//         QGCLabel {
//             text:               "NO ANALOG STREAM DATA"
//             font.weight:        Font.DemiBold
//             color:              "white"
//             font.pixelSize:     _mainIsMap ? 12 * ScreenTools.fontHRatio : 20 * ScreenTools.fontHRatio
//             anchors.centerIn:   parent
//         }
//     }

    QGCViewPanel {
        id:             panel
        anchors.fill:   parent

        Rectangle {
            id: backGround
            color: Qt.rgba(0,0,0,0.75)
            anchors.fill:   parent

            QGCLabel {
                id:             titleLabel
                text:           "Collision Avoidance Camera"
                font.weight:    Font.DemiBold
            }

            Row {
                id:                       buttonRow
                spacing:                  ScreenTools.defaultFontPixelWidth
                anchors.margins:          _margins
                anchors.top:              parent.top
                anchors.horizontalCenter: parent.horizontalCenter

                Rectangle {
                    id: container; 
                    anchors.centerIn: parent
                    function toggle() {
                        if (state=="Start") {state = "Stop"} else { state ="Start"}
                    }
                    state: "Start"
                    states: [
                        State {
                            name: "Start";
                        },
                        State {
                            name: "Stop";
                        }
                    ]
                    QGCButton { 
                        id:             btn; 
                        text:           container.state;
                        width:          ScreenTools.defaultFontPixelWidth * 11
                        // anchors.fill:   parent; 
                        onClicked: {
                            container.toggle();
                            _caController.onStartStopClicked(container.state==="Stop"); 
                        }
                    }
                }
            }
        }

        // Image {
        //     source:     _activeVehicle ? "image://OwnFlow/" + _activeVehicle.id + "/" + _activeVehicle.flowImageIndex : ""
        //     width:      parent.width * 0.5
        //     height:     width * 0.75
        //     cache:      false
        //     anchors.centerIn: parent
        //     fillMode: Image.PreserveAspectFit
        // }
    }
}
