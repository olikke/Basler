import QtQuick 2.11
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.3

import "qrc:/../QML/Style/"

CheckBox {
    id: control
    checked: true

    indicator: Rectangle {
        width: parent.height-4
        height: parent.height-7
        anchors.verticalCenter:  parent.verticalCenter
        radius: Style.unitRadius/2
        color: control.checked? Style.currentTheme.accent  :Style.currentTheme.primary
    }
}
