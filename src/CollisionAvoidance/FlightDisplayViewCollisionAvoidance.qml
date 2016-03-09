import QtQuick                      2.4
import QtQuick.Controls             1.3

import QGroundControl               1.0
import QGroundControl.FlightMap     1.0
import QGroundControl.ScreenTools   1.0
import QGroundControl.Controls      1.0
import QGroundControl.Palette       1.0
import QGroundControl.Vehicle       1.0
//import QGroundControl.Controllers   1.0


Item {
    id: root
    Rectangle {
        id:             noVideo
        anchors.fill:   parent
        color:          Qt.rgba(0,0,0,0.75)
//        visible:        !_controller.hasCollisionAvoidanceStream
        QGCLabel {
            text:               "NO STREAM DATA"
            font.weight:        Font.DemiBold
            color:              "white"
            font.pixelSize:     _mainIsMap ? 12 * ScreenTools.fontHRatio : 20 * ScreenTools.fontHRatio
            anchors.centerIn:   parent
        }
    }
//    QGCVideoBackground {
//        anchors.fill:   parent
//        display:        _controller.videoSurface
//        receiver:       _controller.videoReceiver
//        visible:        _controller.videoRunning
//        runVideo:       true
//    }
}

