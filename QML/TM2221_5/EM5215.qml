import QtQuick 2.11
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.3

import "qrc:/../QML/ControlsSet/"
import "qrc:/../QML/CustomControls/"
import "qrc:/../QML/Style/"

Item {

    Component.onDestruction: {
        em5215.startParse(false)
    }


    property var em5215 : null

    property int oneElementHeight: 20

    property bool em5215DE : em5215.dataExh

    property bool em5215AutoMode : em5215.autoMode

    property real fanConst : em5215.getFanConst()

    property real fanLoValue : em5215.getFanLoValue()

    property real currentLoValue : em5215.getCurrentLoValue()

    property real currentConst : em5215.getCurrentConst()

    Component.onCompleted: initialize()

    function initialize()
    {
        em5215.startParse(true)
        var IntArray=em5215.getFanLimit()
        fanU.minValue=IntArray[0]
        fanU.maxValue=IntArray[1]
        IntArray=em5215.getCurrentLimit()
        currentP1.minValue=IntArray[0]
        currentP1.maxValue=IntArray[1]
        currentP2.minValue=IntArray[0]
        currentP2.maxValue=IntArray[1]
    }

    function setAutoMode(value)
    {
        em5215.setAutoMode(value)
    }

    function switchKu(value)
    {
        em5215.switchKu(value)
    }

    function changeFanU()
    {
        if (fan.isChecked) em5215.changeFanU(fanU.curValue)
        else em5215.changeFanU(0)
    }

    function fanUcalc()
    {
        if (fan.isChecked) return (fanLoValue+fanU.curValue*fanConst).toFixed(2)
        return 0
    }

    function switchR1(value)
    {
        em5215.switchR1(value)
    }

    function switchR2(value)
    {
        em5215.switchR2(value)
    }

    function switchR(value)
    {
        em5215.switchR(value)
    }

    function switchP1()
    {
        em5215.switchP1(p1.curValue,currentP1.curValue)
    }

    function switchP2()
    {
        em5215.switchP2(p2.curValue,currentP2.curValue)
    }

    function currentP1Calc(value)
    {
        if (p1.curValue===0) return 0
        return (currentLoValue+value*currentConst).toFixed(2)
    }

    function currentP2Calc(value)
    {
        if (p2.curValue===0) return 0
        return (currentLoValue+value*currentConst).toFixed(2)
    }

    Column {
        id: column
        anchors.topMargin: 0
        anchors.margins: 10
        anchors.fill: parent
        spacing: 2
        enabled: em5215DE

        CustomLabel {
            text: "Контроллер управления ЭМ-5215-2 (А5)"
        }



        Column {
            width: parent.width
            spacing: 2

            TextPanel{
                panelName: "Обмен данными"
                panelValue: em5215DE? "есть": "нет"
                stateColor: em5215DE? Style.unitGreen : Style.unitRed
                width: parent.width
                height: oneElementHeight
            }

            TextPanel {
                panelName: "Серийный номер"
                panelValue: em5215.serialNumber.toString()
                width: parent.width
                height: oneElementHeight
                panelEnabled: em5215DE
            }

            RealSwitchPanel {
                property bool temp: em5215AutoMode
                onTempChanged: isChecked=temp
                panelName: "Автоматический режим"
                width: parent.width
                height: oneElementHeight
                updateValueFunction: setAutoMode
                isChecked: em5215AutoMode
                panelEnabled: em5215DE
            }

            RealSwitchPanel {
                property bool temp: em5215.isKuOn
                onTempChanged: isChecked=temp
                panelName: "Питание камеры"
                width: parent.width
                height: oneElementHeight
                updateValueFunction: switchKu
                isChecked: temp
                panelEnabled: em5215DE
            }

            RealSwitchPanel {
                property bool temp: em5215.isFanOn
                onTempChanged: isChecked=temp
                id: fan
                panelName: "Вентилятор"
                width: parent.width
                height: oneElementHeight
                updateValueFunction: changeFanU
                isChecked: temp
                enabled: !em5215AutoMode
                panelEnabled: em5215DE
            }

            IntegerSliderPanel {
                property int temp: em5215.fanU
                onTempChanged: curValue=temp
                id: fanU
                panelName: "U питания вент., В"
                width: parent.width
                height: oneElementHeight
                curValue: temp
                updateValueFunction: changeFanU
                enabled: !em5215AutoMode && fan.isChecked
                panelEnabled: em5215DE
                calcValue: fanUcalc
            }

            TextPanel{
                panelEnabled: em5215DE
                panelName: "Обороты вентилятора"
                panelValue: em5215.fanSpeed.toString(10)
                width: parent.width
                height: oneElementHeight
            }



            RealSwitchPanel {
                property bool temp: em5215.isR1On
                onTempChanged: isChecked=temp
                panelName: "Резистор R1"
                width: parent.width
                height: oneElementHeight
                updateValueFunction: switchR1
                isChecked: temp
                enabled: !em5215AutoMode
                panelEnabled: em5215DE
            }

            RealSwitchPanel {
                property bool temp: em5215.isR2On
                onTempChanged: isChecked=temp
                panelName: "Резистор R2"
                width: parent.width
                height: oneElementHeight
                updateValueFunction: switchR2
                isChecked: temp
                enabled: !em5215AutoMode
                panelEnabled: em5215DE
            }

            RealSwitchPanel {
                property bool temp: em5215.isR2On & em5215.isR1On
                onTempChanged: isChecked=temp
                panelName: "Резисторы R1 + R2"
                width: parent.width
                height: oneElementHeight
                updateValueFunction: switchR
                isChecked: temp
                enabled: !em5215AutoMode
                panelEnabled: em5215DE
            }

            ComboBoxPanel {
                property int temp: em5215.isP1On
                onTempChanged: curValue=temp
                id: p1
                panelEnabled: em5215DE
                panelName: "Режим Пельтье 1"
                width: parent.width
                height: oneElementHeight
                customModel: ["выкл", "вкл/охлаждение", "вкл/обогрев"]
                updateValueFunction: switchP1
                enabled: !em5215AutoMode
                curValue: temp
            }

            IntegerSliderPanel {
                property int temp: em5215.currentP1
                onTempChanged: curValue=temp
                panelEnabled: em5215DE
                id: currentP1
                panelName: "Ток Пельтье 1, A"
                width: parent.width
                height: oneElementHeight
                curValue: temp
                updateValueFunction: switchP1
                enabled: !em5215AutoMode
                calcValue: currentP1Calc
            }

            ComboBoxPanel {
                property int temp: em5215.isP2On
                onTempChanged: curValue=temp
                id: p2
                panelEnabled: em5215DE
                panelName: "Режим Пельтье 2"
                width: parent.width
                height: oneElementHeight
                customModel: ["выкл", "вкл/охлаждение", "вкл/обогрев"]
                updateValueFunction: switchP2
                enabled: !em5215AutoMode
                curValue: temp
            }

            IntegerSliderPanel {
                property int temp: em5215.currentP2
                onTempChanged: curValue=temp
                panelEnabled: em5215DE
                id: currentP2
                panelName: "Ток Пельтье 2, A"
                width: parent.width
                height: oneElementHeight
                curValue: temp
                updateValueFunction: switchP2
                calcValue: currentP2Calc
                enabled: !em5215AutoMode
            }

            TextPanel{
                panelEnabled: em5215DE
                panelName: "Т. внутри КУ, C°"
                panelValue: em5215.temperatureKU.toString(10)
                width: parent.width
                height: oneElementHeight
            }
            TextPanel{
                panelEnabled: em5215DE
                panelName: "Т. корпуса КУ, C°"
                panelValue: em5215.temperatureBody.toString()
                width: parent.width
                height: oneElementHeight
            }
            TextPanel{
                panelEnabled: em5215DE
                panelName: "Т. радиатора, C°"
                panelValue: em5215.temperatureRadiator.toString()
                width: parent.width
                height: oneElementHeight
            }
            TextPanel{
                property int temp: em5215.humidity
                onTempChanged: {
                    switch (em5215.getHumidityColor()) {
                    case 0: stateColor=Style.unitGreen; break
                    case 1: stateColor=Style.unitYellow; break
                    case 2: stateColor=Style.unitRed; break
                    }
                }
                panelEnabled: em5215DE
                panelName: "Влажность, %"
                panelValue: temp.toString()
                width: parent.width
                height: oneElementHeight
                stateColor: Style.unitGreen
            }
        }
    }
}
