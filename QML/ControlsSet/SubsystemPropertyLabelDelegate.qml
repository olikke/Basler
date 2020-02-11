import QtQuick 2.0

import "qrc:/../QML/Style/"
import "qrc:/../QML/ControlsSet/"
import "qrc:/../QML/CustomControls/"

Item {
    id:root
    width: parent.width
    implicitHeight: 28
    height: 28

    property bool textAccentColor: false
    property bool backgroundVisibility:true
    property alias propertyName: label.text

    Rectangle {
        id: rectangle
        color: Style.currentTheme.background
        anchors.fill: parent
        radius: Style.unitRadius/2
        visible: backgroundVisibility
    }

    CustomLabel{
        id:label
        font.capitalization: Font.AllUppercase
        anchors.leftMargin: 10
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        color: textAccentColor ? Style.currentTheme.accent : Style.currentTheme.foreground
    }

}
