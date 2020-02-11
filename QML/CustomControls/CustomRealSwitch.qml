import QtQuick 2.11
import QtQuick.Controls 2.2

import "qrc:/../QML/Style/"

Switch {
    id: control

    indicator: Rectangle {
        x: border.width
        implicitWidth: parent.width
        implicitHeight: parent.height
        border.color: Style.currentTheme.background
        border.width: 2
        radius: 13
        color: control.checked ? Style.currentTheme.accent : Style.currentTheme.primary

        Rectangle {
            x: control.checked ? parent.width - width : 0
            width: parent.height
            height: parent.height
            radius: 13
            color: control.down ? Qt.lighter(Style.currentTheme.primary,1.1) : Qt.lighter(Style.currentTheme.primary,1.4)
        }
    }
}
