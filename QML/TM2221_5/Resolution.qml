import QtQuick 2.11
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.3

import "qrc:/../QML/ControlsSet/"
import "qrc:/../QML/CustomControls/"
import "qrc:/../QML/Style/"

Item {
    width: parent.width
    height: column.implicitHeight+5

    Column {
        id: column
        anchors.topMargin: 5
        anchors.margins: 10
        anchors.fill: parent
        spacing: 5

        CustomLabel {
            text: "Разрешение"
            //color: Style.currentTheme.accent
        }

        SwitchTwoItem{
            width: parent.width
            textLeft: "960x540"
            textRight: "1920x1080"
            height: 20
            index: 1
            onIndexChanged: controller.resolutionChange()
        }
    }
}
