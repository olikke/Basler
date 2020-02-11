import QtQuick 2.0

import "qrc:/../QML/CustomControls/"
import "qrc:/../QML/Style/"

Item {
    id:root
    implicitHeight: 30
    implicitWidth: 200
    width: 300
    height: 30

    property alias textLeft: leftText.text
    property alias textMiddle: middleText.text
    property alias textRight: rightText.text
    property int index: 0

    Item {
        id: item1
        width: root.width/3
        anchors.bottom: parent.bottom
        anchors.top: parent.top
        clip: true

        Rectangle {
            color: index !== 0 ? Style.currentTheme.background : Style.currentTheme.accent
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
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                index = 0;
            }
        }
    }

    Item {
        id: item2
        anchors.left: item1.right
        clip: true
        anchors.right: item3.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom

        Rectangle {
            color: index !== 1 ? Style.currentTheme.background : Style.currentTheme.accent
            anchors.leftMargin: -10
            clip: true
            anchors.fill: parent
        }

        CustomLabel {
            id: middleText
            text: "text 2"
            font.capitalization: Font.AllUppercase
            anchors.fill: parent
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                index = 1;
            }
        }
    }

    Item {
        id: item3
        width: root.width/3
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.top: parent.top
        clip: true

        Rectangle {
            color: index !== 2 ? Style.currentTheme.background : Style.currentTheme.accent
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
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                index = 2;
            }
        }
    }


}
