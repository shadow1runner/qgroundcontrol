/****************************************************************************
 *
 *   (c) 2009-2016 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/


import QtQuick                  2.5
import QtQuick.Controls         1.3
import QtQuick.Controls.Styles  1.2
import QtQuick.Dialogs          1.2
import QtLocation               5.3
import QtPositioning            5.2
import QtMultimedia             5.5

import QGroundControl               1.0
import QGroundControl.FlightDisplay 1.0
import QGroundControl.FlightMap     1.0
import QGroundControl.ScreenTools   1.0
import QGroundControl.Controls      1.0
import QGroundControl.Palette       1.0
import QGroundControl.Vehicle       1.0
import QGroundControl.Controllers   1.0
import QGroundControl.FactSystem    1.0

import HW.Controllers   1.0

/// Flight Display View
QGCView {
    id:             root
    viewPanel:      _panel

    QGCPalette { id: qgcPal; colorGroupEnabled: enabled }

    property var _activeVehicle:        QGroundControl.multiVehicleManager.activeVehicle
    property bool _mainIsMap:           QGroundControl.videoManager.hasVideo ? QGroundControl.loadBoolGlobalSetting(_mainIsMapKey,  true) : true
    property string _flightMapContainerState:           QGroundControl.loadGlobalSetting(_flightMapContainerStateKey, "full")
    property string _flightVideoState:                  QGroundControl.loadGlobalSetting(_flightVideoStateKey, "leftPip")
    property string _flightCollisionAvoidanceState:     QGroundControl.loadGlobalSetting(_flightCollisionAvoidanceStateKey, "rightPip")

    property real _roll:                _activeVehicle ? _activeVehicle.roll.value    : _defaultRoll
    property real _pitch:               _activeVehicle ? _activeVehicle.pitch.value   : _defaultPitch
    property real _heading:             _activeVehicle ? _activeVehicle.heading.value : _defaultHeading


    property Fact _emptyFact:               Fact { }
    property Fact _groundSpeedFact:         _activeVehicle ? _activeVehicle.groundSpeed      : _emptyFact
    property Fact _airSpeedFact:            _activeVehicle ? _activeVehicle.airSpeed         : _emptyFact

    property bool activeVehicleJoystickEnabled: _activeVehicle ? _activeVehicle.joystickEnabled : false

    property real _savedZoomLevel:      0

    property real pipSize:              mainWindow.width * 0.2

    readonly property bool      isBackgroundDark:       _mainIsMap ? (_flightMap ? _flightMap.isSatelliteMap : true) : true
    readonly property real      _defaultRoll:           0
    readonly property real      _defaultPitch:          0
    readonly property real      _defaultHeading:        0
    readonly property real      _defaultAltitudeAMSL:   0
    readonly property real      _defaultGroundSpeed:    0
    readonly property real      _defaultAirSpeed:       0
    readonly property string    _mapName:               "FlightDisplayView"
    readonly property string    _showMapBackgroundKey:  "/showMapBackground"
    readonly property string    _mainIsMapKey:          "MainFlyWindowIsMap"
    readonly property string _LeftPipVisibleKey:                "IsPIPVisible"
    readonly property string _RightPipVisibleKey:               "IsCollisionPIPVisible"
    readonly property string _flightMapContainerStateKey:       "FlightMapContainerState"
    readonly property string _flightVideoStateKey:              "FlightVideoState"
    readonly property string _flightCollisionAvoidanceStateKey: "FlightCollisionAvoidanceState"

    property bool _isLeftPipVisible:                    QGroundControl.loadBoolGlobalSetting(_LeftPipVisibleKey, true)
    property bool _isRightPipVisible:                   QGroundControl.loadBoolGlobalSetting(_RightPipVisibleKey, true)

    CollisionAvoidanceController { id: _caController }

    function doStateTransition(sender) { // sender can either be `_leftPipControl` or `_rightPipControl`
        var mode = (sender===_rightPipControl ? "rightPip" : "leftPip")
        var maximizeMe  = findComponentByState(mode)
        var minimizeMe = findComponentByState("full")

        maximizeMe.state = "full"
        minimizeMe.state = mode

        _mainIsMap = maximizeMe===_flightMapContainer
        if(_mainIsMap) {
            // flight map gets maximized -> restore zoom (or save if not yet present)
            if(_savedZoomLevel != 0)
                _flightMap.zoomLevel = _savedZoomLevel
            else
                _savedZoomLevel = _flightMap.zoomLevel
        } else {
            // flight map gets minimized -> zoom out to have a better overview on the minimized map
            _savedZoomLevel = _flightMap.zoomLevel
            _flightMap.zoomLevel = _savedZoomLevel - 3
        }

        QGroundControl.saveGlobalSetting(_flightMapContainerStateKey, _flightMapContainer.state)
        QGroundControl.saveGlobalSetting(_flightVideoStateKey, _flightVideo.state)
        QGroundControl.saveGlobalSetting(_flightCollisionAvoidanceStateKey, _flightCollisionAvoidance.state)
        QGroundControl.saveBoolGlobalSetting(_mainIsMapKey, _mainIsMap)
    }

    function findComponentByState(state) { // returns one of _flightMapContainer, _flightVideo or _flightCollisionAvoidance - only one can have the searched-for `state`
        if(_flightMapContainer.state===state)
            return _flightMapContainer
        if(_flightCollisionAvoidance.state===state)
            return _flightCollisionAvoidance
        if(_flightVideo.state===state)
             return _flightVideo
    }

    function px4JoystickCheck() {
        if ( _activeVehicle && !_activeVehicle.supportsManualControl && (QGroundControl.virtualTabletJoystick || _activeVehicle.joystickEnabled)) {
            px4JoystickSupport.open()
        }
    }

    function setPipVisibility(id, state) {
        if(id===_rightPipControl) {
            _isRightPipVisible = state;
            QGroundControl.saveBoolGlobalSetting(_RightPipVisibleKey, state)
        } else if(id===_leftPipControl) {
            _isLeftPipVisible = state;
            QGroundControl.saveBoolGlobalSetting(_LeftPipVisibleKey, state)
        }
    }

    MessageDialog {
        id:     px4JoystickSupport
        text:   qsTr("Joystick support requires MAVLink MANUAL_CONTROL support. ") +
                qsTr("The firmware you are running does not normally support this. ") +
                qsTr("It will only work if you have modified the firmware to add MANUAL_CONTROL support.")
    }

    Connections {
        target: QGroundControl.multiVehicleManager
        onActiveVehicleChanged: px4JoystickCheck()
    }

    Connections {
        target: QGroundControl
        onVirtualTabletJoystickChanged: px4JoystickCheck()
    }

    onActiveVehicleJoystickEnabledChanged: px4JoystickCheck()

    Component.onCompleted: {
        widgetsLoader.source = "FlightDisplayViewWidgets.qml"
        // init states
        _flightMapContainer.state =       _flightMapContainerState
        _flightCollisionAvoidance.state = _flightCollisionAvoidanceState
        _flightVideo.state =              _flightVideoState

        px4JoystickCheck()
    }

    QGCViewPanel {
        id:             _panel
        anchors.fill:   parent

        //-- Map View
        //   For whatever reason, if FlightDisplayViewMap is the _panel item, changing
        //   width/height has no effect.
        Item {
            id:                             _flightMapContainer
            anchors.margins:                0
            states: [
                State {
                    name:   "leftPip"
                    PropertyChanges {
                        target:             _flightMapContainer
                        anchors.fill:       _leftPipControl
                        z:                  _panel.z + 2
                        visible:            _isLeftPipVisible
                    }
                },
                State {
                    name:   "full"
                    PropertyChanges {
                        target:             _flightMapContainer
                        anchors.fill:       _panel
                        z:                  _panel.z + 1
                        visible:            true
                    }
                },
                State {
                    name:   "rightPip"
                    PropertyChanges {
                        target:             _flightMapContainer
                        anchors.fill:       _rightPipControl
                        z:                  _panel.z + 2
                        visible:            _isRightPipVisible
                    }
                }
            ]
            FlightDisplayViewMap {
                id:             _flightMap
                anchors.fill:   parent
                flightWidgets:  widgetsLoader.item
            }
        }

        //-- Video View
        Item {
            id:             _flightVideo
            // z:              _mainIsMap ? _panel.z + 2 : _panel.z + 1
            // width:          !_mainIsMap ? _panel.width  : pipSize
            // height:         !_mainIsMap ? _panel.height : pipSize * (9/16)
            // anchors.left:   _panel.left
            // anchors.bottom: _panel.bottom
            // visible:        QGroundControl.videoManager.hasVideo && (!_mainIsMap || _isPipVisible)
            states: [
                State {
                    name:   "leftPip"
                    PropertyChanges {
                        target:             _flightVideo
                        anchors.fill:       _leftPipControl
                        z:                  _panel.z + 2
                        visible:            _controller.hasVideo && _isLeftPipVisible
                    }
                },
                State {
                    name:   "full"
                    PropertyChanges {
                        target:             _flightVideo
                        anchors.fill:       _panel
                        z:                  _panel.z + 1
                        visible:            true
                    }
                },
                State {
                    name:   "rightPip"
                    PropertyChanges {
                        target:             _flightVideo
                        anchors.fill:       _rightPipControl
                        z:                  _panel.z + 2
                        visible:            _controller.hasVideo && _isRightPipVisible
                    }
                }
            ]
            //-- Video Streaming
            FlightDisplayViewVideo {
                anchors.fill:   parent
                visible:        QGroundControl.videoManager.isGStreamer
            }
            //-- UVC Video (USB Camera or Video Device)
            Loader {
                id:             cameraLoader
                anchors.fill:   parent
                visible:        !QGroundControl.videoManager.isGStreamer && ((parent.state==="leftPip" && _isLeftPipVisible) || (parent.state==="rightPip" && _isRightPipVisible)) 
                source:         QGroundControl.videoManager.uvcEnabled ? "qrc:/qml/FlightDisplayViewUVC.qml" : "qrc:/qml/FlightDisplayViewDummy.qml"
            }
        }

        //-- Collision Avoidance View
        FlightDisplayViewCollisionAvoidance {
            id:                             _flightCollisionAvoidance
            anchors.margins:                0
            states: [
                State {
                    name:   "leftPip"
                    PropertyChanges {
                        target:             _flightCollisionAvoidance
                        anchors.fill:       _leftPipControl
                        z:                  _panel.z + 2
                        visible:            _caController.hasCollisionAvoidanceStream && _isLeftPipVisible
                    }
                },
                State {
                    name:   "full"
                    PropertyChanges {
                        target:             _flightCollisionAvoidance
                        anchors.fill:       _panel
                        z:                  _panel.z + 1
                        visible:            true
                    }
                },
                State {
                    name:   "rightPip"
                    PropertyChanges {
                        target:             _flightCollisionAvoidance
                        anchors.fill:       _rightPipControl
                        z:                  _panel.z + 2
                        visible:            _caController.hasCollisionAvoidanceStream && _isRightPipVisible
                    }
                }
            ]            
        }

        QGCPipable {
            id:                 _leftPipControl
            z:                  _flightVideo.z + 3
            width:              mainWindow.width * 0.2
            height:             mainWindow.width * 0.2 * (9/16)
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
            width:              mainWindow.width * 0.2
            height:             mainWindow.width * 0.2 * (9/16)
            anchors.left:       _isLeftPipVisible ? _leftPipControl.right : _panel.left
            anchors.margins:    ScreenTools.defaultFontPixelHeight
            anchors.leftMargin: _isLeftPipVisible ? 5 : 50
            anchors.bottom:     _panel.bottom
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
            id:             widgetsLoader
            z:              _panel.z + 4
            height:         ScreenTools.availableHeight
            anchors.left:   parent.left
            anchors.right:  parent.right
            anchors.bottom: parent.bottom
            asynchronous:   true
            visible:        status == Loader.Ready

            property bool isBackgroundDark: root.isBackgroundDark
            property var qgcView: root
        }

        //-- Virtual Joystick
        Loader {
            id:                         virtualJoystickMultiTouch
            z:                          _panel.z + 5
            width:                      parent.width  - (_rightPipControl.width / 2)
            height:                     Math.min(parent.height * 0.25, ScreenTools.defaultFontPixelWidth * 16)
            visible:                    QGroundControl.virtualTabletJoystick
            anchors.bottom:             _rightPipControl.top
            anchors.bottomMargin:       ScreenTools.defaultFontPixelHeight * 2
            anchors.horizontalCenter:   widgetsLoader.horizontalCenter
            source:                     "qrc:/qml/VirtualJoystick.qml"
            active:                     QGroundControl.virtualTabletJoystick

            property bool useLightColors: root.isBackgroundDark
        }
    }
}
