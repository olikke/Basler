import QtQuick 2.11
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.3

import "qrc:/../QML/Style/"
import "qrc:/../QML/ControlsSet/"
import "qrc:/../QML/CustomControls/"

Item {
    property string panelName: "name"
    property var updateValueFunction: null
    property int minValue: 0
    property int maxValue: 6
    property int curValue: 1
    property int step: 1
    property bool panelEnabled : true
    property var calcValue : null

    Row {
        spacing: 2
        anchors.margins: 0
        anchors.fill: parent

        Rectangle {
            width: parent.width/2
            color: Style.currentTheme.background
            height: parent.height
            radius: Style.unitRadius /2

            CustomLabel{
                text: panelName
                anchors.left: parent.left
                anchors.leftMargin: 10
                width: parent.width
                horizontalAlignment: Text.AlignLeft
            }
        }

        Rectangle {
            width: parent.width/2
            color: Style.currentTheme.background
            height: parent.height
            radius: Style.unitRadius /2

            CustomLabel{
                visible: panelEnabled
                id: label
                text:(calcValue)? calcValue(slider.value) :(slider.value).toString(10)
                width: 46
                anchors.left: parent.left
                anchors.leftMargin: 10
                horizontalAlignment: Text.AlignLeft
            }

            CustomSlider {
                visible: panelEnabled
                id: slider
                anchors.right: parent.right
                anchors.rightMargin: 10
                anchors.left: label.right
                anchors.leftMargin: 10
                value: curValue
                stepSize: step
                to: maxValue
                from: minValue
                anchors.verticalCenter: parent.verticalCenter
                onMoved: {
                    curValue=value
                    if (updateValueFunction) updateValueFunction(value)
                }
            }
        }
    }
}
