import QtQuick 2.11
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.3

import "qrc:/../QML/Style/"
import "qrc:/../QML/ControlsSet/"
import "qrc:/../QML/CustomControls/"

Item {
    id: item1
    width: parent.width
    property string panelName: "name"
    property var updateValueFunction: null
    property bool isChecked: false
    property bool panelEnabled: true

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
            CustomRealSwitch{
                visible: panelEnabled
                width: 40
                height: parent.height*0.8
                checked: isChecked
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 10
                Material.accent: parent.enabled? Style.currentTheme.accent : Style.currentTheme.background
                Material.theme: Material.Dark
                onClicked: {
                    isChecked=checked
                    if (updateValueFunction) updateValueFunction(checked)
                }
            }


        }
    }
}
