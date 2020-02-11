import QtQuick 2.11
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.3

import "qrc:/../QML/Style/"

TextField {
    id: control
    placeholderText: qsTr("0.00")
    font.family: Style.robotoCondensed
    font.pointSize: 10
    font.bold: true
    color: enabled? Style.currentTheme.foreground : Style.currentTheme.primary
    bottomPadding: 0
    topPadding: 0
    //verticalAlignment: Text.AlignVCenter
    font.capitalization: Font.AllUppercase
    Material.accent: Style.currentTheme.accent
    leftPadding: -50


    background: Rectangle {
        radius: Style.unitRadius/2
        width: parent.width
        height: parent.height-4
        anchors.verticalCenter: parent.verticalCenter
        color: Style.currentTheme.background
        border.color: control.enabled ? Style.currentTheme.foreground: Style.currentTheme.primary
    }
}
