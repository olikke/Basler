import QtQuick 2.11
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.3

import "qrc:/../QML/ControlsSet/"
import "qrc:/../QML/CustomControls/"
import "qrc:/../QML/Style/"

//обычная кнопка при Button.enabled=false не видна на фоне
Item {

    property string name: "text"
    property var link: null
    property bool enabledd: true

    Rectangle {
        anchors.fill: parent
        color: Style.currentTheme.background
        radius: Style.unitRadius/2

        Button {
            enabled: enabledd
            text: name
            width: parent.width
            height: oneElementHeight+12
            onClicked: if (link) link()
            y: -6
            padding: 0
            highlighted: false
            hoverEnabled: true
            font.bold: true
            font.family: Style.robotoCondensed
            Material.elevation: 0
            Material.background: Style.currentTheme.background
        }


    }

}
