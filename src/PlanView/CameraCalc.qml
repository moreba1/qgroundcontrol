import QtQuick          2.3
import QtQuick.Controls 1.2
import QtQuick.Layouts  1.2

import QGroundControl                   1.0
import QGroundControl.ScreenTools       1.0
import QGroundControl.Controls          1.0
import QGroundControl.FactControls      1.0
import QGroundControl.Palette           1.0

// Camera calculator section for mission item editors
Column {
    anchors.left:   parent.left
    anchors.right:  parent.right
    spacing:        _margin

    property var cameraCalc

    property alias exclusiveGroup:  cameraSectionHeader.exclusiveGroup
    property alias showSpacer:      cameraSectionHeader.showSpacer
    property alias checked:         cameraSectionHeader.checked

    property var    _camera:        missionItem.cameraSection
    property real   _fieldWidth:    ScreenTools.defaultFontPixelWidth * 16
    property real   _margin:        ScreenTools.defaultFontPixelWidth / 2


    property real   _margin:            ScreenTools.defaultFontPixelWidth / 2
    property int    _cameraIndex:       1
    property real   _fieldWidth:        ScreenTools.defaultFontPixelWidth * 10.5
    property var    _cameraList:        [ qsTr("Manual (no camera specs)"), qsTr("Custom Camera") ]
    property var    _vehicle:           QGroundControl.multiVehicleManager.activeVehicle ? QGroundControl.multiVehicleManager.activeVehicle : QGroundControl.multiVehicleManager.offlineEditingVehicle
    property var    _vehicleCameraList: _vehicle ? _vehicle.staticCameraList : []

    readonly property int _gridTypeManual:          0
    readonly property int _gridTypeCustomCamera:    1
    readonly property int _gridTypeCamera:          2

    Component.onCompleted: {
        for (var i=0; i<_vehicle.staticCameraList.length; i++) {
            _cameraList.push(_vehicle.staticCameraList[i].name)
        }
        gridTypeCombo.model = _cameraList
        if (missionItem.manualGrid.value) {
            gridTypeCombo.currentIndex = _gridTypeManual
        } else {
            var index = -1
            for (index=0; index<_cameraList.length; index++) {
                if (_cameraList[index] == missionItem.camera.value) {
                    break;
                }
            }
            missionItem.cameraOrientationFixed = false
            if (index == _cameraList.length) {
                gridTypeCombo.currentIndex = _gridTypeCustomCamera
            } else {
                gridTypeCombo.currentIndex = index
                if (index != 1) {
                    // Specific camera is selected
                    var camera = _vehicleCameraList[index - _gridTypeCamera]
                    missionItem.cameraOrientationFixed = camera.fixedOrientation
                    missionItem.cameraMinTriggerInterval = camera.minTriggerInterval
                }
            }
        }
    }

    QGCPalette { id: qgcPal; colorGroupEnabled: true }

    ExclusiveGroup {
        id: cameraOrientationGroup

        onCurrentChanged: {
            if (gridTypeCombo.currentIndex >= _gridTypeCustomCamera) {
                recalcFromCameraValues()
            }
        }
    }

    ExclusiveGroup { id: fixedValueGroup }

    SectionHeader {
        id:         cameraHeader
        text:       qsTr("Camera")
        showSpacer: false
    }

    Column {
        anchors.left:   parent.left
        anchors.right:  parent.right
        spacing:        _margin
        visible:        cameraHeader.checked

        QGCComboBox {
            id:             gridTypeCombo
            anchors.left:   parent.left
            anchors.right:  parent.right
            model:          _cameraList
            currentIndex:   -1

            onActivated: {
                if (index == _gridTypeManual) {
                    missionItem.manualGrid.value = true
                    missionItem.fixedValueIsAltitude.value = true
                } else if (index == _gridTypeCustomCamera) {
                    missionItem.manualGrid.value = false
                    missionItem.camera.value = gridTypeCombo.textAt(index)
                    missionItem.cameraOrientationFixed = false
                    missionItem.cameraMinTriggerInterval = 0
                } else {
                    missionItem.manualGrid.value = false
                    missionItem.camera.value = gridTypeCombo.textAt(index)
                    _noCameraValueRecalc = true
                    var listIndex = index - _gridTypeCamera
                    missionItem.cameraSensorWidth.rawValue          = _vehicleCameraList[listIndex].sensorWidth
                    missionItem.cameraSensorHeight.rawValue         = _vehicleCameraList[listIndex].sensorHeight
                    missionItem.cameraResolutionWidth.rawValue      = _vehicleCameraList[listIndex].imageWidth
                    missionItem.cameraResolutionHeight.rawValue     = _vehicleCameraList[listIndex].imageHeight
                    missionItem.cameraFocalLength.rawValue          = _vehicleCameraList[listIndex].focalLength
                    missionItem.cameraOrientationLandscape.rawValue = _vehicleCameraList[listIndex].landscape ? 1 : 0
                    missionItem.cameraOrientationFixed              = _vehicleCameraList[listIndex].fixedOrientation
                    missionItem.cameraMinTriggerInterval            = _vehicleCameraList[listIndex].minTriggerInterval
                    _noCameraValueRecalc = false
                    recalcFromCameraValues()
                }
            }
        }

        RowLayout {
            anchors.left:   parent.left
            anchors.right:  parent.right
            spacing:        _margin
            visible:        missionItem.manualGrid.value == true

            QGCCheckBox {
                id:                 cameraTriggerDistanceCheckBox
                anchors.baseline:   cameraTriggerDistanceField.baseline
                text:               qsTr("Trigger Distance")
                checked:            missionItem.cameraTriggerDistance.rawValue > 0
                onClicked: {
                    if (checked) {
                        missionItem.cameraTriggerDistance.value = missionItem.cameraTriggerDistance.defaultValue
                    } else {
                        missionItem.cameraTriggerDistance.value = 0
                    }
                }
            }

            FactTextField {
                id:                 cameraTriggerDistanceField
                Layout.fillWidth:   true
                fact:               missionItem.cameraTriggerDistance
                enabled:            cameraTriggerDistanceCheckBox.checked
            }
        }
    }

    // Camera based grid ui
    Column {
        anchors.left:   parent.left
        anchors.right:  parent.right
        spacing:        _margin
        visible:        gridTypeCombo.currentIndex != _gridTypeManual

        Row {
            spacing:                    _margin
            anchors.horizontalCenter:   parent.horizontalCenter
            visible:                    !missionItem.cameraOrientationFixed

            QGCRadioButton {
                width:          _editFieldWidth
                text:           "Landscape"
                checked:        !!missionItem.cameraOrientationLandscape.value
                exclusiveGroup: cameraOrientationGroup
                onClicked:      missionItem.cameraOrientationLandscape.value = 1
            }

            QGCRadioButton {
                id:             cameraOrientationPortrait
                text:           "Portrait"
                checked:        !missionItem.cameraOrientationLandscape.value
                exclusiveGroup: cameraOrientationGroup
                onClicked:      missionItem.cameraOrientationLandscape.value = 0
            }
        }

        Column {
            id:             custCameraCol
            anchors.left:   parent.left
            anchors.right:  parent.right
            spacing:        _margin
            visible:        gridTypeCombo.currentIndex === _gridTypeCustomCamera

            RowLayout {
                anchors.left:   parent.left
                anchors.right:  parent.right
                spacing:        _margin
                Item { Layout.fillWidth: true }
                QGCLabel {
                    Layout.preferredWidth:  _root._fieldWidth
                    text:                   qsTr("Width")
                }
                QGCLabel {
                    Layout.preferredWidth:  _root._fieldWidth
                    text:                   qsTr("Height")
                }
            }

            RowLayout {
                anchors.left:   parent.left
                anchors.right:  parent.right
                spacing:        _margin
                QGCLabel { text: qsTr("Sensor"); Layout.fillWidth: true }
                FactTextField {
                    Layout.preferredWidth:  _root._fieldWidth
                    fact:                   missionItem.cameraSensorWidth
                }
                FactTextField {
                    Layout.preferredWidth:  _root._fieldWidth
                    fact:                   missionItem.cameraSensorHeight
                }
            }

            RowLayout {
                anchors.left:   parent.left
                anchors.right:  parent.right
                spacing:        _margin
                QGCLabel { text: qsTr("Image"); Layout.fillWidth: true }
                FactTextField {
                    Layout.preferredWidth:  _root._fieldWidth
                    fact:                   missionItem.cameraResolutionWidth
                }
                FactTextField {
                    Layout.preferredWidth:  _root._fieldWidth
                    fact:                   missionItem.cameraResolutionHeight
                }
            }

            RowLayout {
                anchors.left:   parent.left
                anchors.right:  parent.right
                spacing:        _margin
                QGCLabel {
                    text:                   qsTr("Focal length")
                    Layout.fillWidth:       true
                }
                FactTextField {
                    Layout.preferredWidth:  _root._fieldWidth
                    fact:                   missionItem.cameraFocalLength
                }
            }

        } // Column - custom camera

        RowLayout {
            anchors.left:   parent.left
            anchors.right:  parent.right
            spacing:        _margin
            Item { Layout.fillWidth: true }
            QGCLabel {
                Layout.preferredWidth:  _root._fieldWidth
                text:                   qsTr("Front Lap")
            }
            QGCLabel {
                Layout.preferredWidth:  _root._fieldWidth
                text:                   qsTr("Side Lap")
            }
        }

        RowLayout {
            anchors.left:   parent.left
            anchors.right:  parent.right
            spacing:        _margin
            QGCLabel { text: qsTr("Overlap"); Layout.fillWidth: true }
            FactTextField {
                Layout.preferredWidth:  _root._fieldWidth
                fact:                   missionItem.frontalOverlap
            }
            FactTextField {
                Layout.preferredWidth:  _root._fieldWidth
                fact:                   missionItem.sideOverlap
            }
        }

        QGCLabel {
            wrapMode:               Text.WordWrap
            text:                   qsTr("Select one:")
            Layout.preferredWidth:  parent.width
            Layout.columnSpan:      2
        }

        QGCRadioButton {
            id:                     fixedAltitudeRadio
            text:                   qsTr("Altitude")
            checked:                !!missionItem.fixedValueIsAltitude.value
            exclusiveGroup:         fixedValueGroup
            onClicked:              missionItem.fixedValueIsAltitude.value = 1
        }

        FactTextField {
            fact:                   missionItem.gridAltitude
            enabled:                fixedAltitudeRadio.checked
            Layout.fillWidth:       true
        }

        QGCRadioButton {
            id:                     fixedGroundResolutionRadio
            text:                   qsTr("Ground res")
            checked:                !missionItem.fixedValueIsAltitude.value
            exclusiveGroup:         fixedValueGroup
            onClicked:              missionItem.fixedValueIsAltitude.value = 0
        }

        FactTextField {
            fact:                   missionItem.groundResolution
            enabled:                fixedGroundResolutionRadio.checked
            Layout.fillWidth:       true
        }
    }
}
}
