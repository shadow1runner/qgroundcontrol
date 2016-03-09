/*=====================================================================

QGroundControl Open Source Ground Control Station

(c) 2009, 2015 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>

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

import QtQuick                  2.5
import QtQuick.Controls         1.3
import QtQuick.Controls.Styles  1.2
import QtQuick.Dialogs          1.2
import QtLocation               5.3
import QtPositioning            5.2

import QGroundControl               1.0
import QGroundControl.FlightDisplay 1.0
import QGroundControl.FlightMap     1.0
import QGroundControl.ScreenTools   1.0
import QGroundControl.Controls      1.0
import QGroundControl.Palette       1.0
import QGroundControl.Vehicle       1.0
import QGroundControl.Controllers   1.0
import QGroundControl.FactSystem    1.0

/// Flight Display View
QGCView {
    id:             root
    viewPanel:      _panel
    topDialogMargin: height - availableHeight

    QGCPalette { id: qgcPal; colorGroupEnabled: enabled }

    property real availableHeight: parent.height

    readonly property bool isBackgroundDark: _mainIsMap ? (_flightMap ? _flightMap.isSatelliteMap : true) : true

    property var _activeVehicle:    multiVehicleManager.activeVehicle

    readonly property real _defaultRoll:                0
    readonly property real _defaultPitch:               0
    readonly property real _defaultHeading:             0
    readonly property real _defaultAltitudeAMSL:        0
    readonly property real _defaultGroundSpeed:         0
    readonly property real _defaultAirSpeed:            0

    readonly property string _mapName:                  "FlightDisplayView"
    readonly property string _showMapBackgroundKey:     "/showMapBackground"
    readonly property string _mainIsMapKey:             "MainFlyWindowIsMap"
    readonly property string _LeftPipVisibleKey:        "IsPIPVisible"
    readonly property string _RightPipVisibleKey:       "IsCollisionPIPVisible"

    property bool _mainIsMap:                           QGroundControl.loadBoolGlobalSetting(_mainIsMapKey,  true)
    property bool _isLeftPipVisible:                    QGroundControl.loadBoolGlobalSetting(_LeftPipVisibleKey, true)
    property bool _isRightPipVisible:                   QGroundControl.loadBoolGlobalSetting(_RightPipVisibleKey, true)

    property real _roll:                _activeVehicle ? _activeVehicle.roll.value    : _defaultRoll
    property real _pitch:               _activeVehicle ? _activeVehicle.pitch.value   : _defaultPitch
    property real _heading:             _activeVehicle ? _activeVehicle.heading.value : _defaultHeading


    property Fact _emptyFact:               Fact { }
    property Fact _groundSpeedFact:         _activeVehicle ? _activeVehicle.groundSpeed      : _emptyFact
    property Fact _airSpeedFact:            _activeVehicle ? _activeVehicle.airSpeed         : _emptyFact

    property bool activeVehicleJoystickEnabled: _activeVehicle ? _activeVehicle.joystickEnabled : false

    property real _savedZoomLevel:      0

    property real pipSize:              mainWindow.width * 0.2

    FlightDisplayViewController { id: _controller }

    function doStateTransition(sender) { // sender can either be `_leftPipControl` ("LEFT") or `_rightPipControl` ("RIGHT") or undefined (INIT only)
        if(!sender) {
            // INIT
            _flightMapContainer.state = "full"
            _flightVideo.state = "leftPip"
            _flightCollisionAvoidance.state = "rightPip"
            return
        }

        var leftClicked = sender==_leftPipControl // otherwise it must have been the right PIP

        if(_flightMapContainer.state==="full" && _flightVideo.state==="leftPip" && _flightCollisionAvoidance.state==="rightPip") {
            if(leftClicked) {
                _flightMapContainer.state = "leftPip"
                _flightVideo.state = "full"
                _flightCollisionAvoidance.state = "rightPip"
            } else {
                _flightMapContainer.state = "rightPip"
                _flightVideo.state = "leftPip"
                _flightCollisionAvoidance.state = "full"
            }
        } else if(_flightMapContainer.state==="leftPip" && _flightVideo.state==="full" && _flightCollisionAvoidance.state==="rightPip") {
            if(leftClicked) {
                _flightMapContainer.state = "full"
                _flightVideo.state = "leftPip"
                _flightCollisionAvoidance.state = "rightPip"
            } else {
                _flightMapContainer.state = "leftPip"
                _flightVideo.state = "rightPip"
                _flightCollisionAvoidance.state = "full"
            }
        } else if(_flightMapContainer.state==="leftPip" && _flightVideo.state==="rightPip" && _flightCollisionAvoidance.state==="full") {
            if(leftClicked) {
                _flightMapContainer.state = "full"
                _flightVideo.state = "rightPip"
                _flightCollisionAvoidance.state = "leftPip"
            } else {
                _flightMapContainer.state = "leftPip"
                _flightVideo.state = "full"
                _flightCollisionAvoidance.state = "rightPip"
            }
        } else if(_flightMapContainer.state==="full" && _flightVideo.state==="rightPip" && _flightCollisionAvoidance.state==="leftPip") {
            if(leftClicked) {
                _flightMapContainer.state = "leftPip"
                _flightVideo.state = "rightPip"
                _flightCollisionAvoidance.state = "full"
            } else {
                _flightMapContainer.state = "rightPip"
                _flightVideo.state = "full"
                _flightCollisionAvoidance.state = "leftPip"
            }
        } else if(_flightMapContainer.state==="rightPip" && _flightVideo.state==="full" && _flightCollisionAvoidance.state==="leftPip") {
            if(leftClicked) {
                _flightMapContainer.state = "rightPip"
                _flightVideo.state = "leftPip"
                _flightCollisionAvoidance.state = "full"
            } else {
                _flightMapContainer.state = "full"
                _flightVideo.state = "rightPip"
                _flightCollisionAvoidance.state = "leftPip"
            }
        } else if(_flightMapContainer.state==="rightPip" && _flightVideo.state==="leftPip" && _flightCollisionAvoidance.state==="full") {
            if(leftClicked) {
                _flightMapContainer.state = "rightPip"
                _flightVideo.state = "full"
                _flightCollisionAvoidance.state = "leftPip"
            } else {
                _flightMapContainer.state = "full"
                _flightVideo.state = "leftPip"
                _flightCollisionAvoidance.state = "rightPip"
            }
        }

        if(!_mainIsMap && _flightMapContainer.state==="full") {
            // flight map gets maximized -> restore zoom (or save if not yet present)
            if(_savedZoomLevel != 0)
                _flightMap.zoomLevel = _savedZoomLevel
            else
                _savedZoomLevel = _flightMap.zoomLevel

        } else if(_mainIsMap && _flightMapContainer.state!=="full") {
            // flight map gets minimized
            _savedZoomLevel = _flightMap.zoomLevel
            _flightMap.zoomLevel = _savedZoomLevel - 3
        }

        _mainIsMap = _flightMapContainer.state==="full"
        QGroundControl.saveBoolGlobalSetting(_mainIsMapKey, _mainIsMap)
    }

    function px4JoystickCheck() {
        if (_activeVehicle && !_activeVehicle.px4Firmware && (QGroundControl.virtualTabletJoystick || _activeVehicle.joystickEnabled)) {
            px4JoystickSupport.open()
        }
    }

    function setPipVisibility(id, state) {
        if(id===_leftPipControl) {
            _isLeftPipVisible = state;
            QGroundControl.saveBoolGlobalSetting(_LeftPipVisibleKey, state)
        } else if(id===_rightPipControl) {
            _isRightPipVisible = state;
            QGroundControl.saveBoolGlobalSetting(_RightPipVisibleKey, state)
        }
    }

    MessageDialog {
        id:     px4JoystickSupport
        text:   "Joystick support requires MAVLink MANUAL_CONTROL support. " +
                "The firmware you are running does not normally support this. " +
                "It will only work if you have modified the firmware to add MANUAL_CONTROL support."
    }

    Connections {
        target: multiVehicleManager
        onActiveVehicleChanged: px4JoystickCheck()
    }

    Connections {
        target: QGroundControl
        onVirtualTabletJoystickChanged: px4JoystickCheck()
    }

    onActiveVehicleJoystickEnabledChanged: px4JoystickCheck()

    Component.onCompleted: {
        widgetsLoader.source = "FlightDisplayViewWidgets.qml"
        doStateTransition(undefined)
        px4JoystickCheck()
    }

    QGCViewPanel {
        id:             _panel
        anchors.fill:   parent

        //-- Map View
        //   For whatever reason, if FlightDisplayViewMap is the _panel item, changing
        //   width/height has no effect.
        Item {
            id: _flightMapContainer
            anchors.bottom: _panel.bottom
            states: [
                State {
                    name:   "leftPip"
                    PropertyChanges {
                        target:             _flightMapContainer
                        anchors.margins:    ScreenTools.defaultFontPixelHeight
                        anchors.leftMargin: 0
                        anchors.left:       _leftPipControl.left
                        width:              pipSize
                        height:             pipSize * (9/16)
                        z:                  _panel.z + 2
                        visible:            _isLeftPipVisible
                    }
                },
                State {
                    name:   "full"
                    PropertyChanges {
                        target:             _flightMapContainer
                        anchors.margins:    0
                        anchors.left:       _panel.left
                        width:              _panel.width
                        height:             _panel.height
                        z:                  _panel.z + 1
                        visible:            true
                    }
                },
                State {
                    name:   "rightPip"
                    PropertyChanges {
                        target:             _flightMapContainer
                        anchors.margins:    ScreenTools.defaultFontPixelHeight
                        anchors.leftMargin: 0
                        anchors.left:       _rightPipControl.left
                        width:              pipSize
                        height:             pipSize * (9/16)
                        z:                  _panel.z + 2
                        visible:            _isRightPipVisible
                    }
                }
            ]
            FlightDisplayViewMap {
                id:             _flightMap
                anchors.fill:   parent
            }
        }

        //-- Video View
        FlightDisplayViewVideo {
            id:             _flightVideo
            anchors.bottom: _panel.bottom
            states: [
                State {
                    name:   "leftPip"
                    PropertyChanges {
                        target:             _flightVideo
                        anchors.margins:    ScreenTools.defaultFontPixelHeight
                        anchors.leftMargin: 0
                        anchors.left:       _leftPipControl.left
                        width:              pipSize
                        height:             pipSize * (9/16)
                        z:                  _panel.z + 2
                        visible:            _controller.hasVideo && _isLeftPipVisible
                    }
                },
                State {
                    name:   "full"
                    PropertyChanges {
                        target:             _flightVideo
                        anchors.margins:    0
                        anchors.left:       _panel.left
                        width:              _panel.width
                        height:             _panel.height
                        z:                  _panel.z + 1
                        visible:            true
                    }
                },
                State {
                    name:   "rightPip"
                    PropertyChanges {
                        target:             _flightVideo
                        anchors.margins:    ScreenTools.defaultFontPixelHeight
                        anchors.leftMargin: 0
                        anchors.left:       _rightPipControl.left
                        width:              pipSize
                        height:             pipSize * (9/16)
                        z:                  _panel.z + 2
                        visible:            _controller.hasVideo && _isRightPipVisible
                    }
                }
            ]
        }

        //-- Collision Avoidance View
        FlightDisplayViewCollisionAvoidance {
            id:             _flightCollisionAvoidance
            anchors.bottom: _panel.bottom
            states: [
                State {
                    name:   "leftPip"
                    PropertyChanges {
                        target:             _flightCollisionAvoidance
                        anchors.margins:    ScreenTools.defaultFontPixelHeight
                        anchors.leftMargin: 0
                        anchors.left:       _leftPipControl.left
                        width:              pipSize
                        height:             pipSize * (9/16)
                        z:                  _panel.z + 2
                        visible:            _isLeftPipVisible
                    }
                },
                State {
                    name:   "full"
                    PropertyChanges {
                        target:             _flightCollisionAvoidance
                        anchors.margins:    0
                        anchors.left:       _panel.left
                        width:              _panel.width
                        height:             _panel.height
                        z:                  _panel.z + 1
                        visible:            true
                    }
                },
                State {
                    name:   "rightPip"
                    PropertyChanges {
                        target:             _flightCollisionAvoidance
                        anchors.margins:    ScreenTools.defaultFontPixelHeight
                        anchors.leftMargin: 0
                        anchors.left:       _rightPipControl.left
                        width:              pipSize
                        height:             pipSize * (9/16)
                        z:                  _panel.z + 2
                        visible:            _isRightPipVisible
                    }
                }
            ]
        }

        QGCPipable {
            id:                 _leftPipControl
            z:                  _flightVideo.z + 3
            width:              pipSize
            height:             pipSize * (9/16)
            anchors.left:       _panel.left
            anchors.bottom:     _panel.bottom
            anchors.margins:    ScreenTools.defaultFontPixelHeight
            isHidden:           !_isLeftPipVisible
            isDark:             isBackgroundDark
            onActivated: {
                doStateTransition(_leftPipControl)
            }
            onHideIt: {
                setPipVisibility(_leftPipControl, !state)
            }
        }

        QGCPipable {
            id:                 _rightPipControl
            z:                  _flightCollisionAvoidance.z + 3
            width:              pipSize
            height:             pipSize * (9/16)
            anchors.left:       _leftPipControl.right
            anchors.bottom:     _panel.bottom
            anchors.margins:    ScreenTools.defaultFontPixelHeight
            anchors.leftMargin: 5
            isHidden:           !_isRightPipVisible
            isDark:             isBackgroundDark
            onActivated: {
                doStateTransition(_rightPipControl)
            }
            onHideIt: {
                setPipVisibility(_rightPipControl, !state)
            }
        }

        //-- Widgets
        Loader {
            id:                 widgetsLoader
            z:                  _panel.z + 4
            anchors.right:      parent.right
            anchors.left:       parent.left
            anchors.bottom:     parent.bottom
            height:             availableHeight
            asynchronous:       true
            visible:            status == Loader.Ready

            property bool isBackgroundDark: root.isBackgroundDark
            property var qgcView: root
        }

        //-- Virtual Joystick
        Loader {
            id:                         multiTouchItem
            z:                          _panel.z + 5
            width:                      parent.width  - (_leftPipControl.width / 2)
            height:                     Math.min(parent.height * 0.25, ScreenTools.defaultFontPixelWidth * 16)
            visible:                    QGroundControl.virtualTabletJoystick
            anchors.bottom:             _leftPipControl.top
            anchors.bottomMargin:       ScreenTools.defaultFontPixelHeight * 2
            anchors.horizontalCenter:   parent.horizontalCenter
            source:                     "qrc:/qml/VirtualJoystick.qml"
            active:                     QGroundControl.virtualTabletJoystick
        }
    }
}
