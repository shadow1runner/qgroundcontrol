/// @file
///     @author Helmut Wolf <helmut.wolf@student.uibk.ac.at>

import QtQuick                  2.5
import QtQuick.Controls         1.2
import QtQuick.Controls.Styles  1.2
import QtQuick.Dialogs          1.2

import QGroundControl.Palette       1.0
import QGroundControl.Controls      1.0
import QGroundControl.Controllers   1.0
import QGroundControl.ScreenTools   1.0

QGCView {
    viewPanel:  panel

    property real   _margins:    ScreenTools.defaultFontPixelHeight
    QGCPalette                      { id: qgcPal; colorGroupEnabled: enabled }
    CustomCommandWidgetController   { id: controller; factPanel: panel }

    QGCViewPanel {
        id:             panel
        anchors.fill:   parent
        Rectangle {
            anchors.fill:   parent
            color:          qgcPal.window
            QGCLabel {
                id:                 textOutput
                anchors.margins:    _margins
                anchors.left:       parent.left
                anchors.right:      parent.right
                anchors.top:        parent.top
                anchors.bottom:     buttonRow.top
                wrapMode:           Text.WordWrap
                textFormat:         Text.RichText
                text:               "Seas QML"
                visible:            !loader.visible
            }
            Row {
                id:                 buttonRow
                spacing:            ScreenTools.defaultFontPixelWidth
                anchors.margins:    _margins
                anchors.bottom:     parent.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                QGCButton {
                    text:       "Start TBD..."
                    width:      ScreenTools.defaultFontPixelWidth * 22
                    onClicked:  controller.selectQmlFile()
                }
                QGCButton {
                    text:       "Stop TBD..."
                    width:      ScreenTools.defaultFontPixelWidth * 22
                    onClicked: {
                        controller.clearQmlFile()
                        loader.visible  = false
                        textOutput.text = _emptyText
                    }
                }
            }
        }
    }
}


