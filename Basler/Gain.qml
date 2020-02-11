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
                gainAuto.initializionVal()
                gainRaw.initializionVal()
                gainLoLimit.initializionVal()
                gainHiLimit.initializionVal()
                blackLavelRaw.initializionVal()
                gammaEnable.isChecked=myCamera.gammaEnable
                digitalShift.initializionVal()
            }
    }

    function gammaChange()
    {
        if (controlEnabled)  myCamera.gammaEnable=gammaEnable.isChecked
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
            text: "Усиление"
            color: Style.currentTheme.accent
        }

        Column {
            id: column2
            width: parent.width
            spacing: 2
            visible: controlEnabled

            PylonComboBox{
                id: gainAuto
                enabled: true
                customModel: cbItems
                parametername: PylonFeaturesEnum.GainAuto
                width: parent.width
                height: oneElementHeight
            }

            PylonIntegerSlider {
                id: gainRaw
                enabled: gainAuto.curValue!==2
                parametername: PylonFeaturesEnum.GainRaw
                width: parent.width
                height: oneElementHeight
            }

            PylonIntegerSlider {
                id: gainLoLimit
                enabled: gainAuto.curValue!==0
                parametername: PylonFeaturesEnum.GainLoLimit
                link: gainHiLimit.initializionVal
                width: parent.width
                height: oneElementHeight
            }

            PylonIntegerSlider{
                id: gainHiLimit
                enabled: gainAuto.curValue!==0
                parametername: PylonFeaturesEnum.GainHiLimit
                link: gainLoLimit.initializionVal
                width: parent.width
                height: oneElementHeight
            }

            PylonIntegerSlider {
                id: blackLavelRaw
                enabled: true
                parametername: PylonFeaturesEnum.BlackLevelRaw
                width: parent.width
                height: oneElementHeight
            }

            RealSwitchPanel{
                id: gammaEnable
                panelName: "Гамма"
                updateValueFunction: gammaChange
                enabled: true
                width: parent.width
                height: oneElementHeight
            }

            PylonIntegerSlider {
                id: digitalShift
                enabled: true
                parametername: PylonFeaturesEnum.DigitalShift
                width: parent.width
                height: oneElementHeight
            }
        }
    }
}
