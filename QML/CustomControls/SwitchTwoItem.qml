import QtQuick 2.0

import "qrc:/../QML/CustomControls/"
import "qrc:/../QML/Style/"

Item {
    id:root
    implicitHeight: 22
    implicitWidth: 200
    width: 300
    height: 22

    property alias textLeft: leftText.text
    property alias textRight: rightText.text
    property int index: 0

    Item {
        clip: true
        anchors.right: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.top: parent.top

        Rectangle {
            color: index > 0 ? Style.currentTheme.background : Style.currentTheme.accent
            radius: Style.unitRadius/2
            anchors.rightMargin: -10
            anchors.fill: parent

            Behavior on color {
                ColorAnimation{
                   duration: Style.animationDuration/2
                }
            }
        }

        CustomLabel{
            id: leftText
            text: "text 1"
            font.capitalization: Font.AllUppercase
            font.bold: true
            anchors.fill: parent
            color: index > 0 ? Style.currentTheme.foreground : Style.currentTheme.background
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                index = 0;
            }
        }
    }

    Item {
        clip: true
        anchors.bottom: parent.bottom
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.left: parent.horizontalCenter

        Rectangle {
            color: index == 0 ? Style.currentTheme.background : Style.currentTheme.accent
            radius: Style.unitRadius/2
            anchors.leftMargin: -10
            anchors.fill: parent
            clip: true

            Behavior on color {
                ColorAnimation{
                    duration: Style.animationDuration/2
                }
            }
        }

        CustomLabel{
            id: rightText
            text: "text 2"
            font.capitalization: Font.AllUppercase
            anchors.fill: parent
            color: index == 0 ? Style.currentTheme.foreground : Style.currentTheme.background
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                index = 1;
            }
        }
    }

}
