import QtQuick 2.11
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.3
import com.PylonFeaturesEnum 1.0

import "qrc:/../QML/Style/"
import "qrc:/../QML/ControlsSet/"
import "qrc:/../QML/CustomControls/"

Item {

    height: controlEnabled? column.openHeight : column.collapseHeight

    property int oneElementHeight: 20

    Connections
    {
        target: myCamera
        onOpeningChanged:
            if (myCamera.opening)
            {
                eWidth.initializionVal()
                eHeight.initializionVal()
                xOffset.initializionVal()
                yOffset.initializionVal()
                xCenter.isChecked=myCamera.xCenter
                yCenter.isChecked=myCamera.yCenter
            }
    }

    function xCenterChange()
    {
            myCamera.xCenter=xCenter.isChecked
            xOffset.initializionVal()
    }

    function yCenterChange()
    {
            myCamera.yCenter=yCenter.isChecked
            yOffset.initializionVal()
    }

    Column {
        id: column
        anchors.topMargin: 0
        anchors.margins: 10
        anchors.fill: parent
        spacing: 2

        property int openHeight: column2.implicitHeight+caption.height+column.spacing+2
        property int collapseHeight :caption.height

        CustomLabel {
            id: caption
            text: "Размер и положение кадра"
            color: Style.currentTheme.accent
        }

        Column {
            id: column2
            width: parent.width
            spacing: 2
            visible: controlEnabled

            PylonIntegerSlider {
                id: eWidth
                enabled: !myCamera.pylonPlaying
                parametername: PylonFeaturesEnum.Width
                width: parent.width
                height: oneElementHeight
            }

            PylonIntegerSlider {
                id:eHeight
                enabled: !myCamera.pylonPlaying
                parametername: PylonFeaturesEnum.Height
                width: parent.width
                height: oneElementHeight
            }

            PylonIntegerSlider {
                id: xOffset
                enabled: !myCamera.pylonPlaying && !xCenter.isChecked
                parametername: PylonFeaturesEnum.XOffset
                width: parent.width
                height: oneElementHeight
            }

            PylonIntegerSlider {
                id: yOffset
                enabled: !myCamera.pylonPlaying && !yCenter.isChecked
                parametername: PylonFeaturesEnum.YOffset
                width: parent.width
                height: oneElementHeight
            }

            RealSwitchPanel{
                id: xCenter
                panelName: "X центр"
                width: parent.width
                height: oneElementHeight
                updateValueFunction: xCenterChange
                panelEnabled:  !myCamera.pylonPlaying
            }

            RealSwitchPanel {
                id: yCenter
                panelName: "Y центр"
                width: parent.width
                height: oneElementHeight
                updateValueFunction: yCenterChange
                panelEnabled:  !myCamera.pylonPlaying
            }
        }
    }
}
