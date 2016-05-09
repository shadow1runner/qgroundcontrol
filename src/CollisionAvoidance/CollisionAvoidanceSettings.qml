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
                text:   qsTr("Collision Avoidance Settings")
                font.pixelSize: ScreenTools.mediumFontPixelSize
            }
            Rectangle {
                height: 1
                width:  parent.width
                color:  qgcPal.button
            }
            Item {
                height: ScreenTools.defaultFontPixelHeight
                width:  parent.width
            }

            QGCLabel {
                text:   qsTr("Data Source")
                font.pixelSize: ScreenTools.mediumFontPixelSize
            }
            Rectangle {
                height: 1
                width:  parent.width/2
                color:  qgcPal.button
            }
            Item {
                height: ScreenTools.defaultFontPixelHeight / 2
                width:  parent.width
            }

            Row {
                QGCLabel {
                    id:     useRecordedVideoInsteadOfDeviceLabel
                    text:   qsTr("Collision Avoidance Source:")
                    anchors.verticalCenter: parent.verticalCenter
                }

                ListModel {
                    id: fileNameDeviceModel
                    ListElement { text: "Device"; value: 0 }
                    ListElement { text: "Recorded Video"; value: 1 }
                }

                Item {
                    height: ScreenTools.defaultFontPixelHeight / 2
                    width:  20
                    anchors.verticalCenter: parent.verticalCenter
                }

                QGCComboBox {
                    id:                 fileNameDeviceCombo
                    anchors.margins:    ScreenTools.defaultFontPixelWidth
                    anchors.verticalCenter: parent.verticalCenter
                    width:              ScreenTools.defaultFontPixelWidth * 40
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
                    text:   qsTr("File Name:")
                    anchors.verticalCenter: parent.verticalCenter
                }
                
                QGCTextField {
                    id:     fileNameField
                    text:   _controller.fileName
                    width:  ScreenTools.defaultFontPixelWidth * 160
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
                    text:   qsTr("Device ID:")
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
                    text:   qsTr("Rotate incoming frames:")
                    anchors.verticalCenter: parent.verticalCenter
                }

                ListModel {
                    id: rotationModel
                    ListElement { text: "0°"   ; value: 0 }
                    ListElement { text: "90°"  ; value: 1 }
                    ListElement { text: "180°" ; value: 2 }
                    ListElement { text: "270°" ; value: 3 }
                }

                Item {
                    width:  20
                    anchors.verticalCenter: parent.verticalCenter
                }

                QGCComboBox {
                    id:                 rotationCombo
                    anchors.margins:    ScreenTools.defaultFontPixelWidth
                    anchors.verticalCenter: parent.verticalCenter
                    width:              ScreenTools.defaultFontPixelWidth * 8
                    model:              rotationModel
                    currentIndex:       _controller.rawFrameRotation

                    onActivated: {
                      _controller.rawFrameRotation = Boolean(index) //fileNameDeviceModel.get(index).value
                    }
                }
            }

            Item {
                height: ScreenTools.defaultFontPixelHeight
                width:  parent.width
            }

            QGCLabel {
                text:   qsTr("OwnFlow Details")
                font.pixelSize: ScreenTools.mediumFontPixelSize
            }
            Rectangle {
                height: 1
                width:  parent.width/2
                color:  qgcPal.button
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
                    text:   qsTr("Subsample Amount:")
                    anchors.verticalCenter: parent.verticalCenter
                }

                Row {
                    Rectangle {
                        width:              subsampleAmountField.height
                        height:             width
                        color:              qgcPal.button
                        QGCLabel {
                            text:           "-"
                            anchors.centerIn: parent
                        }
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                if(_controller.subsampleAmount > 1)
                                    _controller.subsampleAmount = _controller.subsampleAmount - 1
                            }
                        }
                    }
                    QGCTextField {
                        id:     subsampleAmountField
                        text:   _controller.subsampleAmount.toString()
                        width:              ScreenTools.defaultFontPixelWidth * 12
                        maximumLength:      1
                        validator:          IntValidator {bottom: 1; top: 6;}
                        onEditingFinished: {
                            var subsampleAmount = parseInt(text)
                            if(subsampleAmount >= 1 && subsampleAmount <= 6)
                                _controller.subsampleAmount = subsampleAmount
                        }
                    }
                    Rectangle {
                        width:              subsampleAmountField.height
                        height:             width
                        color:              qgcPal.button
                        QGCLabel {
                            text:           "+"
                            anchors.centerIn: parent
                        }
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                if(_controller.subsampleAmount < 6)
                                    _controller.subsampleAmount = _controller.subsampleAmount + 1
                            }
                        }
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
                    text:   qsTr("Particles:")
                    anchors.verticalCenter: parent.verticalCenter
                }
                QGCTextField {
                    id:     particlesField
                    text:   _controller.particles.toString()
                    width:  ScreenTools.defaultFontPixelWidth * 16
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
                    text:   qsTr("Window Size:")
                    anchors.verticalCenter: parent.verticalCenter
                }
                Row {
                    Rectangle {
                        width:              windowSizeField.height
                        height:             width
                        color:              qgcPal.button
                        QGCLabel {
                            text:           "-"
                            anchors.centerIn: parent
                        }
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                if(_controller.windowSize > 1)
                                    _controller.windowSize = _controller.windowSize - 1
                            }
                        }
                    }
                    QGCTextField {
                        id:     windowSizeField
                        text:   _controller.windowSize.toString()
                        width:  ScreenTools.defaultFontPixelWidth * 12
                        inputMethodHints:       Qt.ImhFormattedNumbersOnly
                        anchors.verticalCenter: parent.verticalCenter
                        showUnits:          true
                        unitsLabel:         "px"
                        validator:          IntValidator {bottom: 1; top: 50;}
                        onEditingFinished: {
                            var windowSize = parseInt(text)
                            if(windowSize >= 1 && windowSize <= 50)
                                _controller.windowSize = windowSize
                        }
                    }
                    Rectangle {
                        width:              windowSizeField.height
                        height:             width
                        color:              qgcPal.button
                        QGCLabel {
                            text:           "+"
                            anchors.centerIn: parent
                        }
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                if(_controller.windowSize < 50)
                                    _controller.windowSize = _controller.windowSize + 1
                            }
                        }
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
                Row {
                    Rectangle {
                        width:              inlierProportionThresholdField.height
                        height:             width
                        color:              qgcPal.button
                        QGCLabel {
                            text:           "-"
                            anchors.centerIn: parent
                        }
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                if(_controller.inlierProportionThreshold > 0.001)
                                    _controller.inlierProportionThreshold = _controller.inlierProportionThreshold - 0.001
                            }
                        }
                    }
                    QGCTextField {
                        id:     inlierProportionThresholdField
                        text:   (_controller.inlierProportionThreshold*1000).toString()
                        width:  ScreenTools.defaultFontPixelWidth * 12
                        inputMethodHints:       Qt.ImhFormattedNumbersOnly
                        anchors.verticalCenter: parent.verticalCenter
                        showUnits:          true
                        unitsLabel:         "‰"
                        validator:          DoubleValidator {bottom: 0.1; top: 50.0; decimals: 2;}
                        onEditingFinished: {
                            var inlierProportionThreshold = parseFloat(text)
                            if(inlierProportionThreshold >= 0.1 && inlierProportionThreshold <= 50.0)
                                _controller.inlierProportionThreshold = inlierProportionThreshold/1000
                        }
                    }
                    Rectangle {
                        width:              inlierProportionThresholdField.height
                        height:             width
                        color:              qgcPal.button
                        QGCLabel {
                            text:           "+"
                            anchors.centerIn: parent
                        }
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                if(_controller.inlierProportionThreshold < 0.50)
                                    _controller.inlierProportionThreshold = _controller.inlierProportionThreshold + 0.001
                            }
                        }
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
                    text:   qsTr("Divergence Patch Size:")
                    anchors.verticalCenter: parent.verticalCenter
                }
                Row {
                    Rectangle {
                        width:              divergencePatchSizeField.height
                        height:             width
                        color:              qgcPal.button
                        QGCLabel {
                            text:           "-"
                            anchors.centerIn: parent
                        }
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                if(_controller.divergencePatchSize > 1)
                                    _controller.divergencePatchSize = _controller.divergencePatchSize - 1
                            }
                        }
                    }
                    QGCTextField {
                        id:     divergencePatchSizeField
                        text:   _controller.divergencePatchSize.toString()
                        width:  ScreenTools.defaultFontPixelWidth * 12
                        inputMethodHints:       Qt.ImhFormattedNumbersOnly
                        anchors.verticalCenter: parent.verticalCenter
                        showUnits:          true
                        unitsLabel:         "px"
                        validator:          IntValidator {bottom: 1; top: 50;}
                        onEditingFinished: {
                            var divergencePatchSize = parseInt(text)
                            if(divergencePatchSize >= 1 && divergencePatchSize <= 50)
                                _controller.divergencePatchSize = divergencePatchSize
                        }
                    }
                    Rectangle {
                        width:              divergencePatchSizeField.height
                        height:             width
                        color:              qgcPal.button
                        QGCLabel {
                            text:           "+"
                            anchors.centerIn: parent
                        }
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                if(_controller.divergencePatchSize < 50)
                                    _controller.divergencePatchSize = _controller.divergencePatchSize + 1
                            }
                        }
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
                    text:   qsTr("Divergence Threshold: (currently unused!)")
                    anchors.verticalCenter: parent.verticalCenter
                }
                Row {
                    Rectangle {
                        width:              divergenceThresholdField.height
                        height:             width
                        color:              qgcPal.button
                        QGCLabel {
                            text:           "-"
                            anchors.centerIn: parent
                        }
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                if(_controller.divergenceThreshold > 0.001)
                                    _controller.divergenceThreshold = _controller.divergenceThreshold - 0.001
                            }
                        }
                    }
                    QGCTextField {
                        id:     divergenceThresholdField
                        text:   (_controller.divergenceThreshold*100).toString()
                        width:  ScreenTools.defaultFontPixelWidth * 12
                        inputMethodHints:       Qt.ImhFormattedNumbersOnly
                        anchors.verticalCenter: parent.verticalCenter
                        showUnits:          true
                        unitsLabel:         "%"
                        validator:          DoubleValidator {bottom: 0.1; top: 100.0; decimals: 2;}
                        onEditingFinished: {
                            var divergenceThreshold = parseFloat(text)
                            if(divergenceThreshold >= 0.1 && divergenceThreshold <= 100.0)
                                _controller.divergenceThreshold = divergenceThreshold/100
                        }
                    }
                    Rectangle {
                        width:              divergenceThresholdField.height
                        height:             width
                        color:              qgcPal.button
                        QGCLabel {
                            text:           "+"
                            anchors.centerIn: parent
                        }
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                if(_controller.divergenceThreshold < 1)
                                    _controller.divergenceThreshold = _controller.divergenceThreshold + 0.001
                            }
                        }
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
                    width:  ScreenTools.defaultFontPixelWidth * 160
                    anchors.verticalCenter: parent.verticalCenter
                    enabled: _controller.undistortFrames
                    onEditingFinished: {
                        _controller.ocamModelPath = ocamModelPathField.text
                    }
                }
            }

            Item {
                height: ScreenTools.defaultFontPixelHeight
                width:  parent.width
            }            

            QGCLabel {
                text:   qsTr("UI Settings")
                font.pixelSize: ScreenTools.mediumFontPixelSize
            }
            Rectangle {
                height: 1
                width:  parent.width/2
                color:  qgcPal.button
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

            Row {
                spacing:    ScreenTools.defaultFontPixelWidth
                QGCLabel {
                    text:   qsTr("Optical Flow Visualization - Factor (dx, dy):")
                    anchors.verticalCenter: parent.verticalCenter
                }
                QGCTextField {
                    id:     opticalFlowVectorVisualizationFactorField
                    text:   _controller.opticalFlowVectorVisualizationFactor.toString()
                    width:  ScreenTools.defaultFontPixelWidth * 12
                    inputMethodHints:       Qt.ImhFormattedNumbersOnly
                    anchors.verticalCenter: parent.verticalCenter
                    onEditingFinished: {
                        _controller.opticalFlowVectorVisualizationFactor = parseFloat(opticalFlowVectorVisualizationFactorField.text)
                    }
                }
            }

            Item {
                height: ScreenTools.defaultFontPixelHeight
                width:  parent.width
            }




            QGCLabel {
                text:   qsTr("Frame Persistence Settings")
                font.pixelSize: ScreenTools.mediumFontPixelSize
            }
            Rectangle {
                height: 1
                width:  parent.width/2
                color:  qgcPal.button
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
                    width:  ScreenTools.defaultFontPixelWidth * 75  
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
                    width:  ScreenTools.defaultFontPixelWidth * 75  
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
                    width:  ScreenTools.defaultFontPixelWidth * 75  
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
                    width:  ScreenTools.defaultFontPixelWidth * 75  
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
                    width:  ScreenTools.defaultFontPixelWidth * 75  
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
                    width:  ScreenTools.defaultFontPixelWidth * 75  
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
