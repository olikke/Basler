import QtQuick 2.11
import QtQuick.Controls 2.2

import "qrc:/../QML/Style/"
import "qrc:/../QML/ControlsSet/"
import "qrc:/../QML/CustomControls/"

Item {

    width: parent.width
    property string panelName: "name"
    property string panelValue: ""
    property bool   panelEnabled: true
    property var  stateColor : Style.currentTheme.background
    onStateColorChanged: {
        stateColor===Style.currentTheme.background?
                    label.color=Style.currentTheme.foreground: label.color=Style.currentTheme.background
    }

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
            color: panelEnabled? stateColor : Style.currentTheme.background
            height: parent.height
            radius: Style.unitRadius /2
            CustomLabel{
                id: label
                visible: panelEnabled
                text: panelValue
                anchors.right: parent.right
                anchors.rightMargin: 10
                width: parent.width
                horizontalAlignment: Text.AlignRight
            }
        }
    }
}
