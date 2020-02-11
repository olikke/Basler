import QtQuick 2.11
import QtQuick.Controls 2.2

import "qrc:/../QML/Style/"
import "qrc:/../QML/ControlsSet/"
import "qrc:/../QML/CustomControls/"

Item {
    property string name: "name"
    property var updateValueFunction: null
    property bool isChecked : true
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
                text: name
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

            CustomCheckBox {
                id: cb
                height: parent.height
                anchors.right: parent.right
                anchors.rightMargin: -10
                checked: isChecked
                onCheckStateChanged: {
                    isChecked=checked
                    if (updateValueFunction) updateValueFunction()
                }

            }
        }
    }
}
