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
                            if(subsampleAmount >= 0 && subsampleAmount <= 6)
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
                    text:   qsTr("Divergence Threshold:")
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
                        validator:          DoubleValidator {bottom: 0.1; top: 100000.0; decimals: 2;}
                        onEditingFinished: {
                            var divergenceThreshold = parseFloat(text)
                            if(divergenceThreshold >= 0.1 && divergenceThreshold <= 100000.0)
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
            //-----------------------------------------------------------------
            //-- avgDivergenceThreshold
            Row {
                spacing:    ScreenTools.defaultFontPixelWidth
                QGCCheckBox {
                    text:       "AVG Divergence Threshold:"
                    anchors.verticalCenter: parent.verticalCenter
                    checked:    _controller.UseAvgDivergenceThreshold
                    onClicked: {
                        _controller.UseAvgDivergenceThreshold = checked
                    }
                }
                Row {
                    enabled:    _controller.UseAvgDivergenceThreshold
                    Rectangle {
                        width:              avgDivergenceThresholdField.height
                        height:             width
                        color:              qgcPal.button
                        QGCLabel {
                            text:           "-"
                            anchors.centerIn: parent
                        }
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                if(_controller.avgDivergenceThreshold > 0.001)
                                    _controller.avgDivergenceThreshold = _controller.avgDivergenceThreshold - 0.001
                            }
                        }
                    }
                    QGCTextField {
                        id:     avgDivergenceThresholdField
                        text:   (_controller.avgDivergenceThreshold*100).toString()
                        width:  ScreenTools.defaultFontPixelWidth * 12
                        inputMethodHints:       Qt.ImhFormattedNumbersOnly
                        anchors.verticalCenter: parent.verticalCenter
                        showUnits:          true
                        unitsLabel:         "%"
                        validator:          DoubleValidator {bottom: 0.1; top: 100000.0; decimals: 2;}
                        onEditingFinished: {
                            var avgDivergenceThreshold = parseFloat(text)
                            if(avgDivergenceThreshold >= 0.1 && avgDivergenceThreshold <= 100000.0)
                                _controller.avgDivergenceThreshold = avgDivergenceThreshold/100
                        }
                    }
                    Rectangle {
                        width:              avgDivergenceThresholdField.height
                        height:             width
                        color:              qgcPal.button
                        QGCLabel {
                            text:           "+"
                            anchors.centerIn: parent
                        }
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                if(_controller.avgDivergenceThreshold < 1)
                                    _controller.avgDivergenceThreshold = _controller.avgDivergenceThreshold + 0.001
                            }
                        }
                    }
                }
            }
            //-----------------------------------------------------------------
            //-- DivergenceHistoryBufferSize
            Row {
                spacing:    ScreenTools.defaultFontPixelWidth
                QGCLabel {
                    text:   qsTr("DivergenceHistoryBufferSize for AVG Divergence (requires restart):")
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
                                if(_controller.divergenceHistoryBufferSize > 1)
                                    _controller.divergenceHistoryBufferSize = _controller.divergenceHistoryBufferSize - 1
                            }
                        }
                    }
                    QGCTextField {
                        id:     divergenceHistoryBufferSizeField
                        text:   _controller.divergenceHistoryBufferSize.toString()
                        width:  ScreenTools.defaultFontPixelWidth * 12
                        inputMethodHints:       Qt.ImhFormattedNumbersOnly
                        anchors.verticalCenter: parent.verticalCenter
                        validator:          IntValidator {bottom: 1; }
                        onEditingFinished: {
                            var tmp = parseInt(text)
                            if(DivergenceHistoryBufferSize >= 1)
                                _controller.divergenceHistoryBufferSize = tmp
                        }
                    }
                    Rectangle {
                        width:              divergenceHistoryBufferSizeField.height
                        height:             width
                        color:              qgcPal.button
                        QGCLabel {
                            text:           "+"
                            anchors.centerIn: parent
                        }
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                _controller.divergenceHistoryBufferSize = _controller.divergenceHistoryBufferSize + 1
                            }
                        }
                    }
                }
            }

            //-----------------------------------------------------------------
            //-- undistortFrames
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

            //-- RoiEnabled
            Row {
                spacing:    ScreenTools.defaultFontPixelWidth
                QGCCheckBox {
                    text:       "Use region of interest (ROI)"
                    anchors.verticalCenter: parent.verticalCenter
                    checked:    _controller.RoiEnabled
                    onClicked: {
                        _controller.RoiEnabled = checked
                    }
                }
                Item {
                    height: ScreenTools.defaultFontPixelHeight / 2
                    width: ScreenTools.defaultFontPixelHeight / 2
                }
            }
            //-----------------------------------------------------------------
            //-- RoiWidthPx
            Row {
                spacing:    ScreenTools.defaultFontPixelWidth
                QGCLabel {
                    text:   qsTr("      Width of ROI (after subsampling):")
                    anchors.verticalCenter: parent.verticalCenter
                }
                Row {
                    enabled:    _controller.RoiEnabled
                    Rectangle {
                        width:              roiWidthPxField.height
                        height:             width
                        color:              qgcPal.button
                        QGCLabel {
                            text:           "-"
                            anchors.centerIn: parent
                        }
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                if(_controller.RoiWidthPx > 1)
                                    _controller.RoiWidthPx = _controller.RoiWidthPx - 1
                            }
                        }
                    }
                    QGCTextField {
                        id:     roiWidthPxField
                        text:   _controller.RoiWidthPx.toString()
                        width:  ScreenTools.defaultFontPixelWidth * 12
                        inputMethodHints:       Qt.ImhFormattedNumbersOnly
                        anchors.verticalCenter: parent.verticalCenter
                        showUnits:          true
                        unitsLabel:         "px"
                        validator:          IntValidator {bottom: 1; top: 500;}
                        onEditingFinished: {
                            var RoiWidthPx = parseInt(text)
                            if(RoiWidthPx >= 1 && RoiWidthPx <= 500)
                                _controller.RoiWidthPx = RoiWidthPx
                        }
                    }
                    Rectangle {
                        width:              roiWidthPxField.height
                        height:             width
                        color:              qgcPal.button
                        QGCLabel {
                            text:           "+"
                            anchors.centerIn: parent
                        }
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                if(_controller.RoiWidthPx < 500)
                                    _controller.RoiWidthPx = _controller.RoiWidthPx + 1
                            }
                        }
                    }
                }
            }
            //-----------------------------------------------------------------
            //-- RoiHeightPx
            Row {
                spacing:    ScreenTools.defaultFontPixelWidth
                QGCLabel {
                    text:   qsTr("      Height of ROI (after subsampling):")
                    anchors.verticalCenter: parent.verticalCenter
                }
                Row {
                    enabled:    _controller.RoiEnabled
                    Rectangle {
                        width:              roiHeightPxField.height
                        height:             width
                        color:              qgcPal.button
                        QGCLabel {
                            text:           "-"
                            anchors.centerIn: parent
                        }
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                if(_controller.RoiHeightPx > 1)
                                    _controller.RoiHeightPx = _controller.RoiHeightPx - 1
                            }
                        }
                    }
                    QGCTextField {
                        id:     roiHeightPxField
                        text:   _controller.RoiHeightPx.toString()
                        width:  ScreenTools.defaultFontPixelWidth * 12
                        inputMethodHints:       Qt.ImhFormattedNumbersOnly
                        anchors.verticalCenter: parent.verticalCenter
                        showUnits:          true
                        unitsLabel:         "px"
                        validator:          IntValidator {bottom: 1; top: 500;}
                        onEditingFinished: {
                            var RoiHeightPx = parseInt(text)
                            if(RoiHeightPx >= 1 && RoiHeightPx <= 500)
                                _controller.RoiHeightPx = RoiHeightPx
                        }
                    }
                    Rectangle {
                        width:              roiHeightPxField.height
                        height:             width
                        color:              qgcPal.button
                        QGCLabel {
                            text:           "+"
                            anchors.centerIn: parent
                        }
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                if(_controller.RoiHeightPx < 500)
                                    _controller.RoiHeightPx = _controller.RoiHeightPx + 1
                            }
                        }
                    }
                }
            }
            //-----------------------------------------------------------------
            //-- MaxRollAngleDegrees
            Row {
                spacing:    ScreenTools.defaultFontPixelWidth
                QGCLabel {
                    text:   qsTr("      Max Roll Lean Angle of Vehicle:")
                    anchors.verticalCenter: parent.verticalCenter
                }
                Row {
                    enabled:    _controller.RoiEnabled
                    Rectangle {
                        width:              maxRollAngleDegreesField.height
                        height:             width
                        color:              qgcPal.button
                        QGCLabel {
                            text:           "-"
                            anchors.centerIn: parent
                        }
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                if(_controller.MaxRollAngleDegrees > -45)
                                    _controller.MaxRollAngleDegrees = _controller.MaxRollAngleDegrees - 1
                            }
                        }
                    }
                    QGCTextField {
                        id:     maxRollAngleDegreesField
                        text:   _controller.MaxRollAngleDegrees.toString()
                        width:  ScreenTools.defaultFontPixelWidth * 12
                        inputMethodHints:       Qt.ImhFormattedNumbersOnly
                        anchors.verticalCenter: parent.verticalCenter
                        showUnits:          true
                        unitsLabel:         "°"
                        validator:          IntValidator {bottom: -45; top: 45;}
                        onEditingFinished: {
                            var MaxRollAngleDegrees = parseInt(text)
                            if(MaxRollAngleDegrees >= -45 && MaxRollAngleDegrees <= 45)
                                _controller.MaxRollAngleDegrees = MaxRollAngleDegrees
                        }
                    }
                    Rectangle {
                        width:              maxRollAngleDegreesField.height
                        height:             width
                        color:              qgcPal.button
                        QGCLabel {
                            text:           "+"
                            anchors.centerIn: parent
                        }
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                if(_controller.MaxRollAngleDegrees < 45)
                                    _controller.MaxRollAngleDegrees = _controller.MaxRollAngleDegrees + 1
                            }
                        }
                    }
                }
            }
            //-----------------------------------------------------------------
            //-- MaxPitchAngleDegrees
            Row {
                spacing:    ScreenTools.defaultFontPixelWidth
                QGCLabel {
                    text:   qsTr("      Max Pitch Lean Angle of Vehicle:")
                    anchors.verticalCenter: parent.verticalCenter
                }
                Row {
                    enabled:    _controller.RoiEnabled
                    Rectangle {
                        width:              maxPitchAngleDegreesField.height
                        height:             width
                        color:              qgcPal.button
                        QGCLabel {
                            text:           "-"
                            anchors.centerIn: parent
                        }
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                if(_controller.MaxPitchAngleDegrees > -45)
                                    _controller.MaxPitchAngleDegrees = _controller.MaxPitchAngleDegrees - 1
                            }
                        }
                    }
                    QGCTextField {
                        id:     maxPitchAngleDegreesField
                        text:   _controller.MaxPitchAngleDegrees.toString()
                        width:  ScreenTools.defaultFontPixelWidth * 12
                        inputMethodHints:       Qt.ImhFormattedNumbersOnly
                        anchors.verticalCenter: parent.verticalCenter
                        showUnits:          true
                        unitsLabel:         "°"
                        validator:          IntValidator {bottom: -45; top: 45;}
                        onEditingFinished: {
                            var MaxPitchAngleDegrees = parseInt(text)
                            if(MaxPitchAngleDegrees >= -45 && MaxPitchAngleDegrees <= 45)
                                _controller.MaxPitchAngleDegrees = MaxPitchAngleDegrees
                        }
                    }
                    Rectangle {
                        width:              maxPitchAngleDegreesField.height
                        height:             width
                        color:              qgcPal.button
                        QGCLabel {
                            text:           "+"
                            anchors.centerIn: parent
                        }
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                if(_controller.MaxPitchAngleDegrees < 45)
                                    _controller.MaxPitchAngleDegrees = _controller.MaxPitchAngleDegrees + 1
                            }
                        }
                    }
                }
            }

            Row {
                QGCLabel {
                    text:           "   "
                }
                spacing:    ScreenTools.defaultFontPixelWidth
                QGCCheckBox {
                    id:     useRoiSolelyForDescendDetectionField
                    text:   "Use ROI solely for Descend detection"
                    anchors.verticalCenter: parent.verticalCenter
                    checked:    _controller.UseRoiSolelyForDescendDetection
                    onClicked: {
                        _controller.UseRoiSolelyForDescendDetection = checked
                    }
                }
            }

            //-----------------------------------------------------------------
            //-- ClimbRateValueThreshold
            Row {
                spacing:    ScreenTools.defaultFontPixelWidth
                QGCLabel {
                    text:   qsTr("      Climb rate threshold:")
                    anchors.verticalCenter: parent.verticalCenter
                }
                Row {
                    enabled:    _controller.RoiEnabled
                    Rectangle {
                        width:              climbRateValueThresholdField.height
                        height:             width
                        color:              qgcPal.button
                        QGCLabel {
                            text:           "-"
                            anchors.centerIn: parent
                        }
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                if(_controller.ClimbRateValueThreshold > 0.11)
                                    _controller.ClimbRateValueThreshold = _controller.ClimbRateValueThreshold - 0.1
                            }
                        }
                    }
                    QGCTextField {
                        id:     climbRateValueThresholdField
                        text:   _controller.ClimbRateValueThreshold.toString()
                        width:  ScreenTools.defaultFontPixelWidth * 12
                        inputMethodHints:       Qt.ImhFormattedNumbersOnly
                        anchors.verticalCenter: parent.verticalCenter
                        showUnits:          true
                        unitsLabel:         "m/s"
                        validator:          DoubleValidator {bottom: 0.01; top: 50.0; decimals: 2;}
                        onEditingFinished: {
                            var ClimbRateValueThreshold = parseFloat(text)
                            if(ClimbRateValueThreshold >= 0.01 && ClimbRateValueThreshold <= 50.0)
                                _controller.ClimbRateValueThreshold = ClimbRateValueThreshold
                        }
                    }
                    Rectangle {
                        width:              climbRateValueThresholdField.height
                        height:             width
                        color:              qgcPal.button
                        QGCLabel {
                            text:           "+"
                            anchors.centerIn: parent
                        }
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                if(_controller.ClimbRateValueThreshold <= 49.9)
                                    _controller.ClimbRateValueThreshold = _controller.ClimbRateValueThreshold + 0.1
                            }
                        }
                    }
                }
            }

            Item {
                height: ScreenTools.defaultFontPixelHeight
                width:  parent.width
            }

            //-- WithholdCollisionAction
            Row {
                spacing:    ScreenTools.defaultFontPixelWidth
                QGCCheckBox {
                    text:       "Withhold Collision Action (Debug only!)"
                    anchors.verticalCenter: parent.verticalCenter
                    checked:    _controller.withholdCollisionAction
                    onClicked: {
                        _controller.withholdCollisionAction = checked
                    }
                }
                Item {
                    height: ScreenTools.defaultFontPixelHeight / 2
                    width: ScreenTools.defaultFontPixelHeight / 2
                }
            }

            //-- AutoResumeAfterCollision
            Row {
                spacing:    ScreenTools.defaultFontPixelWidth
                enabled:    _controller.withholdCollisionAction

                QGCLabel {
                    text:           "   "
                }

                QGCCheckBox {
                    text:       "Auto resume (=reset+start) after collision"
                    anchors.verticalCenter: parent.verticalCenter
                    checked:    _controller.AutoResumeAfterCollision && _controller.withholdCollisionAction
                    onClicked: {
                        _controller.AutoResumeAfterCollision = checked
                    }
                }
            }

            Item {
                height: ScreenTools.defaultFontPixelHeight / 2
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
                    text:       "Display all frames in UI (including skipped ones)"
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
                    text:   qsTr("Optical Flow Visualization Factor (dx, dy):")
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
                QGCLabel {
                    text:           "   "
                }
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
                QGCLabel {
                    text:           "   "
                }
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
                QGCLabel {
                    text:           "   "
                }
                spacing:    ScreenTools.defaultFontPixelWidth
                QGCCheckBox {
                    text:       "Write skipped frames to:"
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
                QGCLabel {
                    text:           "   "
                }
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
                QGCLabel {
                    text:           "   "
                }
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
                QGCLabel {
                    text:           "   "
                }
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
                QGCLabel {
                    text:           "   "
                }
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

            
            //-----------------------------------------------------------------
            //-- csvFilePath
            Row {
                spacing:    ScreenTools.defaultFontPixelWidth
                QGCLabel {
                    text:       "Write CSV log file to:"
                    anchors.verticalCenter: parent.verticalCenter
                }
                Item {
                    height: ScreenTools.defaultFontPixelHeight / 2
                    width: ScreenTools.defaultFontPixelHeight / 2
                }
                QGCTextField {
                    id:     csvFilePath
                    text:   _controller.csvFilePath
                    width:  ScreenTools.defaultFontPixelWidth * 75  
                    anchors.verticalCenter: parent.verticalCenter
                    enabled:    _controller.csvFilePath
                    onEditingFinished: {
                        _controller.csvFilePath = csvFilePath.text
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
