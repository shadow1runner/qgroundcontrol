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
                text:   "Collision Avoidance Settings"
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
            //-- Rotation
            Row {
                QGCLabel {
                    text:   "Rotate incoming frames:"
                }

                ListModel {
                    id: rotationModel
                    ListElement { text: "0°"   ; value: 0 }
                    ListElement { text: "90°"  ; value: 1 }
                    ListElement { text: "180°" ; value: 2 }
                    ListElement { text: "270°" ; value: 3 }
                }

                Item {
                    height: ScreenTools.defaultFontPixelHeight / 2
                    width:  20
                }

                QGCComboBox {
                    id:                 rotationCombo
                    anchors.margins:    ScreenTools.defaultFontPixelWidth
                    width:              ScreenTools.defaultFontPixelWidth * 20
                    model:              rotationModel
                    currentIndex:       _controller.rawFrameRotation

                    onActivated: {
                      _controller.rawFrameRotation = Boolean(index) //fileNameDeviceModel.get(index).value
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

            //-----------------------------------------------------------------
            //-- undistortFrames
            Item {
                height: ScreenTools.defaultFontPixelHeight / 2
                width:  parent.width
            }

            Row {
                spacing:    ScreenTools.defaultFontPixelWidth
                QGCCheckBox {
                    text:       "Undistort Frames - Path to calibration file:"
                    anchors.verticalCenter: parent.verticalCenter
                    checked:    _controller.undistortFrames
                    onClicked: {
                        _controller.undistortFrames = checked
                    }
                }
                Item {
                    height: ScreenTools.defaultFontPixelHeight / 2
                    width: ScreenTools.defaultFontPixelHeight / 2
                }
                QGCTextField {
                    id:     ocamModelPathField
                    text:   _controller.ocamModelPath.toString()
                    width:  ScreenTools.defaultFontPixelWidth * 60
                    anchors.verticalCenter: parent.verticalCenter
                    enabled: _controller.undistortFrames
                    onEditingFinished: {
                        _controller.ocamModelPath = ocamModelPathField.text
                    }
                }
            }

            Item {
                height: ScreenTools.defaultFontPixelHeight / 2
                width:  parent.width
            }            

            //-----------------------------------------------------------------
            //-- DisplayBadFramesInUi
            Row {
                spacing:    ScreenTools.defaultFontPixelWidth
                QGCCheckBox {
                    text:       "Display bad frames in UI"
                    anchors.verticalCenter: parent.verticalCenter
                    checked:    _controller.displayBadFramesInUi
                    onClicked: {
                        _controller.displayBadFramesInUi = checked
                    }
                }
                Item {
                    height: ScreenTools.defaultFontPixelHeight / 2
                    width: ScreenTools.defaultFontPixelHeight / 2
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
                    text:       "Persist frames"
                    anchors.verticalCenter: parent.verticalCenter
                    checked:    _controller.writeToOutputEnabled && _controller.writeToOutputEnabled
                    onClicked: {
                        _controller.writeToOutputEnabled = checked
                    }
                }
                Item {
                    height: ScreenTools.defaultFontPixelHeight / 2
                    width: ScreenTools.defaultFontPixelHeight / 2
                }
            }

            Item {
                height: ScreenTools.defaultFontPixelHeight / 2
                width:  parent.width
            }

            //-----------------------------------------------------------------
            //-- clearOldFramesEnabled
            Row {
                spacing:    ScreenTools.defaultFontPixelWidth
                QGCCheckBox {
                    text:       "Clear old persisted frames on start"
                    anchors.verticalCenter: parent.verticalCenter
                    checked:    _controller.writeToOutputEnabled && _controller.clearOldFramesEnabled
                    enabled:    _controller.writeToOutputEnabled
                    onClicked: {
                        _controller.clearOldFramesEnabled = checked
                    }
                }
                Item {
                    height: ScreenTools.defaultFontPixelHeight / 2
                    width: ScreenTools.defaultFontPixelHeight / 2
                }
            }

            Item {
                height: ScreenTools.defaultFontPixelHeight / 2
                width:  parent.width
            }

            
            //-----------------------------------------------------------------
            //-- writeRawFrames
            Row {
                spacing:    ScreenTools.defaultFontPixelWidth
                QGCCheckBox {
                    text:       "Write raw frames to:"
                    anchors.verticalCenter: parent.verticalCenter
                    checked:    _controller.writeToOutputEnabled && _controller.writeRawFrames
                    enabled:    _controller.writeToOutputEnabled
                    onClicked: {
                        _controller.writeRawFrames = checked
                    }
                }
                Item {
                    height: ScreenTools.defaultFontPixelHeight / 2
                    width: ScreenTools.defaultFontPixelHeight / 2
                }
                QGCTextField {
                    id:     rawFramesPath
                    text:   _controller.rawFramesPath
                    width:  ScreenTools.defaultFontPixelWidth * 60
                    anchors.verticalCenter: parent.verticalCenter
                    enabled:    _controller.writeToOutputEnabled && _controller.writeRawFrames
                    onEditingFinished: {
                        _controller.rawFramesPath = rawFramesPath.text
                    }
                }
            }

            Item {
                height: ScreenTools.defaultFontPixelHeight / 2
                width:  parent.width
            }

            
            //-----------------------------------------------------------------
            //-- writeBadFrames
            Row {
                spacing:    ScreenTools.defaultFontPixelWidth
                QGCCheckBox {
                    text:       "Write bad frames to:"
                    anchors.verticalCenter: parent.verticalCenter
                    checked:    _controller.writeToOutputEnabled && _controller.writeBadFrames
                    enabled:    _controller.writeToOutputEnabled
                    onClicked: {
                        _controller.writeBadFrames = checked
                    }
                }
                Item {
                    height: ScreenTools.defaultFontPixelHeight / 2
                    width: ScreenTools.defaultFontPixelHeight / 2
                }
                QGCTextField {
                    id:     badFramePath
                    text:   _controller.badFramesPath
                    width:  ScreenTools.defaultFontPixelWidth * 60
                    anchors.verticalCenter: parent.verticalCenter
                    enabled:    _controller.writeToOutputEnabled && _controller.writeBadFrames
                    onEditingFinished: {
                        _controller.badFramesPath = badFramePath.text
                    }
                }
            }

            Item {
                height: ScreenTools.defaultFontPixelHeight / 2
                width:  parent.width
            }

            
            //-----------------------------------------------------------------
            //-- writeGoodFrames
            Row {
                spacing:    ScreenTools.defaultFontPixelWidth
                QGCCheckBox {
                    text:       "Write good frames (i.e. with an FoE) to:"
                    anchors.verticalCenter: parent.verticalCenter
                    checked:    _controller.writeToOutputEnabled && _controller.writeGoodFrames
                    enabled:    _controller.writeToOutputEnabled
                    onClicked: {
                        _controller.writeGoodFrames = checked
                    }
                }
                Item {
                    height: ScreenTools.defaultFontPixelHeight / 2
                    width: ScreenTools.defaultFontPixelHeight / 2
                }
                QGCTextField {
                    id:     goodFramesPath
                    text:   _controller.goodFramesPath
                    width:  ScreenTools.defaultFontPixelWidth * 60
                    anchors.verticalCenter: parent.verticalCenter
                    enabled:    _controller.writeToOutputEnabled && _controller.writeGoodFrames
                    onEditingFinished: {
                        _controller.goodFramesPath = goodFramesPath.text
                    }
                }
            }

            Item {
                height: ScreenTools.defaultFontPixelHeight / 2
                width:  parent.width
            }

            
            //-----------------------------------------------------------------
            //-- writeHistogramFrames
            Row {
                spacing:    ScreenTools.defaultFontPixelWidth
                QGCCheckBox {
                    text:       "Write histogram frames to:"
                    anchors.verticalCenter: parent.verticalCenter
                    checked:    _controller.writeToOutputEnabled && _controller.writeHistogramFrames
                    enabled:    _controller.writeToOutputEnabled
                    onClicked: {
                        _controller.writeHistogramFrames = checked
                    }
                }
                Item {
                    height: ScreenTools.defaultFontPixelHeight / 2
                    width: ScreenTools.defaultFontPixelHeight / 2
                }
                QGCTextField {
                    id:     histogramFramesPath
                    text:   _controller.histogramFramesPath
                    width:  ScreenTools.defaultFontPixelWidth * 60
                    anchors.verticalCenter: parent.verticalCenter
                    enabled:    _controller.writeToOutputEnabled && _controller.writeHistogramFrames
                    onEditingFinished: {
                        _controller.histogramFramesPath = histogramFramesPath.text
                    }
                }
            }

            Item {
                height: ScreenTools.defaultFontPixelHeight / 2
                width:  parent.width
            }

            
            //-----------------------------------------------------------------
            //-- writeOpticalFlowFrames
            Row {
                spacing:    ScreenTools.defaultFontPixelWidth
                QGCCheckBox {
                    text:       "Write optical flow frames to:"
                    anchors.verticalCenter: parent.verticalCenter
                    checked:    _controller.writeToOutputEnabled && _controller.writeOpticalFlowFrames
                    enabled:    _controller.writeToOutputEnabled
                    onClicked: {
                        _controller.writeOpticalFlowFrames = checked
                    }
                }
                Item {
                    height: ScreenTools.defaultFontPixelHeight / 2
                    width: ScreenTools.defaultFontPixelHeight / 2
                }
                QGCTextField {
                    id:     opticalFlowFramesPath
                    text:   _controller.opticalFlowFramesPath
                    width:  ScreenTools.defaultFontPixelWidth * 60
                    anchors.verticalCenter: parent.verticalCenter
                    enabled:    _controller.writeToOutputEnabled && _controller.writeOpticalFlowFrames
                    onEditingFinished: {
                        _controller.opticalFlowFramesPath = opticalFlowFramesPath.text
                    }
                }
            }

            Item {
                height: ScreenTools.defaultFontPixelHeight / 2
                width:  parent.width
            }

            
            //-----------------------------------------------------------------
            //-- writeUiFrames
            Row {
                spacing:    ScreenTools.defaultFontPixelWidth
                QGCCheckBox {
                    text:       "Write UI frames to:"
                    anchors.verticalCenter: parent.verticalCenter
                    checked:    _controller.writeToOutputEnabled && _controller.writeUiFrames
                    enabled:    _controller.writeToOutputEnabled
                    onClicked: {
                        _controller.writeUiFrames = checked
                    }
                }
                Item {
                    height: ScreenTools.defaultFontPixelHeight / 2
                    width: ScreenTools.defaultFontPixelHeight / 2
                }
                QGCTextField {
                    id:     uiFramesPath
                    text:   _controller.uiFramesPath
                    width:  ScreenTools.defaultFontPixelWidth * 60
                    anchors.verticalCenter: parent.verticalCenter
                    enabled:    _controller.writeToOutputEnabled && _controller.writeUiFrames
                    onEditingFinished: {
                        _controller.uiFramesPath = uiFramesPath.text
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
