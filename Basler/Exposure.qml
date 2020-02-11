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
                exposureAuto.initializionVal()
                exposureTime.initializionVal()
                enableFrameRate.isChecked=myCamera.rateControl
                acquisitionFramRate.initializionVal()
                exposureTimeHILimit.initializionVal()
                exposureTimeLOLimit.initializionVal()
                updateTextValueF(resultFramePeriod, PylonFeaturesEnum.ResultingFramePeriodAbs) //text
                updateTextValueF(resultFrameRate, PylonFeaturesEnum.ResultingFrameRateAbs)

            }
    }

    function updateTextValueF(mCo, parametername)
    {
        var Array = myCamera.getFloatFeatures(parametername)
        if (Array[0]===0)
        {
            mCo.panelValue="0.0"
        } else
        {
            mCo.panelValue=Array[3].toFixed(2)
        }
    }

    function updateResultFrame()
    {
        updateTextValueF(resultFramePeriod, PylonFeaturesEnum.ResultingFramePeriodAbs)
        updateTextValueF(resultFrameRate, PylonFeaturesEnum.ResultingFrameRateAbs)
    }

    function rateControl()
    {
        if (controlEnabled)  myCamera.rateControl=enableFrameRate.isChecked
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
            text: "Экспозиция"
            color: Style.currentTheme.accent
        }

        Column {
            id: column2
            width: parent.width
            spacing: 2
            visible: controlEnabled

            PylonComboBox {
                id: exposureAuto
                enabled:true
                customModel: cbItems
                parametername: PylonFeaturesEnum.ExposureAuto
                link: exposureTime.initializonVal
                width: parent.width
                height: oneElementHeight
            }

            PylonIntegerSlider{
                id: exposureTime
                enabled: exposureAuto.curValue===0
                parametername: PylonFeaturesEnum.ExposureTimeRaw
                link: updateResultFrame
                width: parent.width
                height: oneElementHeight
            }

            RealSwitchPanel{
                id: enableFrameRate
                panelEnabled: true
                updateValueFunction: rateControl
                panelName: "Задать частоту кадров"
                width: parent.width
                height: oneElementHeight

            }

            PylonFloat{
                id: acquisitionFramRate
                parametername: PylonFeaturesEnum.AcquisitionFrameRateAbs
                width: parent.width
                height: oneElementHeight
                link: updateResultFrame
                enabled: enableFrameRate.isChecked
            }

            TextPanel{
                id: resultFramePeriod
                panelName: "Макс. экспозиция"
                panelValue: ""
                width: parent.width
                height: oneElementHeight
            }

            TextPanel{
                id: resultFrameRate
                panelName: "Макс. кадр/сек"
                panelValue: ""
                width: parent.width
                height: oneElementHeight
            }

            PylonFloat{
                id: exposureTimeLOLimit
                parametername: PylonFeaturesEnum.AutoExposureTimeAbsLowerLimit
                width: parent.width
                height: oneElementHeight
                link:exposureTimeHILimit.initializionVal
                enabled:exposureAuto.curValue!==0
            }

            PylonFloat{
                id: exposureTimeHILimit
                parametername: PylonFeaturesEnum.AutoExposureTimeAbsUpperLimit
                width: parent.width
                height: oneElementHeight
                link:exposureTimeLOLimit.initializionVal
                enabled:exposureAuto.curValue!==0
            }
        }
    }
}
