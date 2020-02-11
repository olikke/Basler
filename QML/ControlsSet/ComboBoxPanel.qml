import QtQuick 2.11
import QtQuick.Controls 2.2

import "qrc:/../QML/Style/"
import "qrc:/../QML/ControlsSet/"
import "qrc:/../QML/CustomControls/"

Item {
    property string panelName: "name"
    property var updateValueFunction: null
    property var customModel: ["1","2", "3"]
    property int curValue: 0
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
        }

        Rectangle {
            width: parent.width/2
            color: Style.currentTheme.background
            height: parent.height
            radius: Style.unitRadius /2
        }
    }


    CustomLabel{
        text: panelName
        anchors.left: parent.left
        anchors.leftMargin: 10
        width: parent.width/2-10
        horizontalAlignment: Text.AlignLeft
    }

    CustomComboBox {
        visible: panelEnabled
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.left: parent.horizontalCenter
        anchors.leftMargin: 10
        model: customModel
        height: parent.height*0.8
        anchors.verticalCenter:  parent.verticalCenter
        currentIndex: curValue
        onActivated: {
            curValue=currentIndex
            if (updateValueFunction) updateValueFunction(currentIndex)
        }
    }
}
