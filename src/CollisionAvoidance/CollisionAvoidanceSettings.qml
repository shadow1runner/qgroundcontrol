import QtQuick                  2.5
import QtQuick.Controls         1.2
import QtQuick.Controls.Styles  1.2
import QtQuick.Dialogs          1.1

import QGroundControl                       1.0
import QGroundControl.FactSystem            1.0
import QGroundControl.FactControls          1.0
import QGroundControl.Controls              1.0
import QGroundControl.ScreenTools           1.0
import QGroundControl.MultiVehicleManager   1.0
import QGroundControl.Palette               1.0
import HW.Controllers                       1.0

Rectangle {
    id:                 _generalRoot
    color:              qgcPal.window
    anchors.fill:       parent
    anchors.margins:    ScreenTools.defaultFontPixelWidth

    CollisionAvoidanceSettingsController { id: _controller }

    QGCPalette { id: qgcPal }

    QGCFlickable {
        clip:               true
        anchors.fill:       parent
        contentHeight:      settingsColumn.height
        contentWidth:       settingsColumn.width

        Column {
            id:                 settingsColumn
            anchors.margins:    ScreenTools.defaultFontPixelWidth
            spacing:            ScreenTools.defaultFontPixelHeight / 2


            QGCLabel {
                text:   "Collision Avoidance Settings (change requires restart)"
                font.pixelSize: ScreenTools.mediumFontPixelSize
            }
            Rectangle {
                height: 1
                width:  parent.width
                color:  qgcPal.button
            }

            Item {
                height: ScreenTools.defaultFontPixelHeight / 2
                width:  parent.width
            }

            Row {
                QGCLabel {
                    id:     useRecordedVideoInsteadOfDeviceLabel
                    text:   "Collision Avoidance Source:"
                }

                ListModel {
                    id: fileNameDeviceModel
                    ListElement { text: "Device"; value: 0 }
                    ListElement { text: "Recorded Video"; value: 1 }
                }

                Item {
                    height: ScreenTools.defaultFontPixelHeight / 2
                    width:  20
                }

                QGCComboBox {
                    id:                 fileNameDeviceCombo
                    anchors.margins:    ScreenTools.defaultFontPixelWidth
                    width:              ScreenTools.defaultFontPixelWidth * 20
                    model:              fileNameDeviceModel
                    currentIndex:       _controller.useRecordedVideoInsteadOfDevice ? 1 : 0

                    onActivated: {
                      _controller.useRecordedVideoInsteadOfDevice = Boolean(index) //fileNameDeviceModel.get(index).value
                    }
                }
            }

            Item {
                height: ScreenTools.defaultFontPixelHeight / 2
                width:  parent.width
            }
            //-----------------------------------------------------------------
            //-- FileName
            Row {
                visible: _controller.useRecordedVideoInsteadOfDevice
                spacing:    ScreenTools.defaultFontPixelWidth
                QGCLabel {
                    text:   "File Name:"
                    anchors.verticalCenter: parent.verticalCenter
                }
                
                QGCTextField {
                    id:     fileNameField
                    text:   _controller.fileName
                    width:  ScreenTools.defaultFontPixelWidth * 60
                    anchors.verticalCenter: parent.verticalCenter
                    onEditingFinished: {
                        _controller.fileName = fileNameField.text
                    }
                }
            }

            Item {
                height: ScreenTools.defaultFontPixelHeight / 2
                width:  parent.width
            }
            //-----------------------------------------------------------------
            //-- Device
            Row {
                visible: !_controller.useRecordedVideoInsteadOfDevice
                spacing:    ScreenTools.defaultFontPixelWidth
                QGCLabel {
                    text:   "Device ID:"
                    anchors.verticalCenter: parent.verticalCenter
                }
                QGCTextField {
                    id:     deviceField
                    text:   _controller.device.toString()
                    width:  ScreenTools.defaultFontPixelWidth * 6
                    inputMethodHints:       Qt.ImhFormattedNumbersOnly
                    anchors.verticalCenter: parent.verticalCenter
                    onEditingFinished: {
                        _controller.device = parseInt(deviceField.text)
                    }
                }
            }

            Item {
                height: ScreenTools.defaultFontPixelHeight / 2
                width:  parent.width
            }
            
            //-----------------------------------------------------------------
            //-- Output Dir
            Row {
                spacing:    ScreenTools.defaultFontPixelWidth
                QGCCheckBox {
                    text:       "Write to output directory (dir has to exist):"
                    anchors.verticalCenter: parent.verticalCenter
                    checked:    _controller.writeToOutputDirEnabled
                    onClicked: {
                        _controller.writeToOutputDirEnabled = checked
                    }
                }
                Item {
                    height: ScreenTools.defaultFontPixelHeight / 2
                    width: ScreenTools.defaultFontPixelHeight / 2
                }
                QGCTextField {
                    id:     outputDirField
                    text:   _controller.outputDir
                    width:  ScreenTools.defaultFontPixelWidth * 60
                    anchors.verticalCenter: parent.verticalCenter
                    enabled: _controller.writeToOutputDirEnabled
                    onEditingFinished: {
                        _controller.outputDir = outputDirField.text
                    }
                }
            }

            Item {
                height: ScreenTools.defaultFontPixelHeight / 2
                width:  parent.width
            }

            //-----------------------------------------------------------------
            //-- subsampleAmount
            Row {
                spacing:    ScreenTools.defaultFontPixelWidth
                QGCLabel {
                    text:   "Subsample Amount:"
                    anchors.verticalCenter: parent.verticalCenter
                }
                QGCTextField {
                    id:     subsampleAmountField
                    text:   _controller.subsampleAmount.toString()
                    width:  ScreenTools.defaultFontPixelWidth * 6
                    inputMethodHints:       Qt.ImhFormattedNumbersOnly
                    anchors.verticalCenter: parent.verticalCenter
                    onEditingFinished: {
                        _controller.subsampleAmount = parseInt(subsampleAmountField.text)
                    }
                }
            }

            Item {
                height: ScreenTools.defaultFontPixelHeight / 2
                width:  parent.width
            }
            //-----------------------------------------------------------------
            //-- particles
            Row {
                spacing:    ScreenTools.defaultFontPixelWidth
                QGCLabel {
                    text:   "Particles:"
                    anchors.verticalCenter: parent.verticalCenter
                }
                QGCTextField {
                    id:     particlesField
                    text:   _controller.particles.toString()
                    width:  ScreenTools.defaultFontPixelWidth * 6
                    inputMethodHints:       Qt.ImhFormattedNumbersOnly
                    anchors.verticalCenter: parent.verticalCenter
                    onEditingFinished: {
                        _controller.particles = parseInt(particlesField.text)
                    }
                }
            }

            Item {
                height: ScreenTools.defaultFontPixelHeight / 2
                width:  parent.width
            }
            //-----------------------------------------------------------------
            //-- windowSize
            Row {
                spacing:    ScreenTools.defaultFontPixelWidth
                QGCLabel {
                    text:   "Window Size:"
                    anchors.verticalCenter: parent.verticalCenter
                }
                QGCTextField {
                    id:     windowSizeField
                    text:   _controller.windowSize.toString()
                    width:  ScreenTools.defaultFontPixelWidth * 6
                    inputMethodHints:       Qt.ImhFormattedNumbersOnly
                    anchors.verticalCenter: parent.verticalCenter
                    onEditingFinished: {
                        _controller.windowSize = parseInt(windowSizeField.text)
                    }
                }
            }

            Item {
                height: ScreenTools.defaultFontPixelHeight / 2
                width:  parent.width
            }
            //-----------------------------------------------------------------
            //-- inlierProportionThreshold
            Row {
                spacing:    ScreenTools.defaultFontPixelWidth
                QGCCheckBox {
                    text:       "Sampling based on Inlier Proportion:"
                    anchors.verticalCenter: parent.verticalCenter
                    checked:    _controller.inlierProportionThresholdEnabled
                    onClicked: {
                        _controller.inlierProportionThresholdEnabled = checked
                    }
                }
                Item {
                    height: ScreenTools.defaultFontPixelHeight / 2
                    width: ScreenTools.defaultFontPixelHeight / 2
                }
                QGCTextField {
                    id:     inlierProportionThresholdField
                    text:   _controller.inlierProportionThreshold.toString()
                    width:  ScreenTools.defaultFontPixelWidth * 6
                    inputMethodHints:       Qt.ImhFormattedNumbersOnly
                    anchors.verticalCenter: parent.verticalCenter
                    enabled: _controller.inlierProportionThresholdEnabled
                    onEditingFinished: {
                        _controller.inlierProportionThreshold = parseFloat(inlierProportionThresholdField.text)
                    }
                }
            }

            Item {
                height: ScreenTools.defaultFontPixelHeight / 2
                width:  parent.width
            }
            //-----------------------------------------------------------------
            //-- divergencePatchSize
            Row {
                spacing:    ScreenTools.defaultFontPixelWidth
                QGCLabel {
                    text:   "Divergence Patch Size:"
                    anchors.verticalCenter: parent.verticalCenter
                }
                QGCTextField {
                    id:     divergencePatchSizeField
                    text:   _controller.divergencePatchSize.toString()
                    width:  ScreenTools.defaultFontPixelWidth * 6
                    inputMethodHints:       Qt.ImhFormattedNumbersOnly
                    anchors.verticalCenter: parent.verticalCenter
                    onEditingFinished: {
                        _controller.divergencePatchSize = parseInt(divergencePatchSizeField.text)
                    }
                }
            }

            Item {
                height: ScreenTools.defaultFontPixelHeight / 2
                width:  parent.width
            }
            //-----------------------------------------------------------------
            //-- divergenceThreshold
            Row {
                spacing:    ScreenTools.defaultFontPixelWidth
                QGCLabel {
                    text:   "Divergence Threshold:"
                    anchors.verticalCenter: parent.verticalCenter
                }
                QGCTextField {
                    id:     divergenceThresholdField
                    text:   _controller.divergenceThreshold.toString()
                    width:  ScreenTools.defaultFontPixelWidth * 6
                    inputMethodHints:       Qt.ImhFormattedNumbersOnly
                    anchors.verticalCenter: parent.verticalCenter
                    onEditingFinished: {
                        _controller.divergenceThreshold = parseFloat(divergenceThresholdField.text)
                    }
                }
            }

            Item {
                height: ScreenTools.defaultFontPixelHeight / 2
                width:  parent.width
            }
        }
    }
}
