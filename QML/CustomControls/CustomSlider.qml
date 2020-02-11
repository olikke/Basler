import QtQuick 2.11
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.3

import "qrc:/../QML/Style/"

Slider {
    id: control
    height: 5
    padding: 0
    value: 0.5

    background: Rectangle {
        id:background
        x: control.leftPadding
        y: control.topPadding + control.availableHeight / 2 - height / 2
        implicitWidth: 200
        implicitHeight: control.height
        width: control.availableWidth
        height: implicitHeight
        color: Style.currentTheme.primary//#bdbebf

        Rectangle {
            id:progressLine
            width: control.visualPosition * parent.width
            height: parent.height
            color:enabled? Style.currentTheme.accent: Qt.lighter(Style.currentTheme.primary,1.4)
        }
    }

    handle: Rectangle {
        id:groove
        x: progressLine.width - width/2
        y: control.topPadding + control.availableHeight / 2 - height / 2
        implicitWidth: control.height*1.7
        implicitHeight: control.height*1.7
        radius: control.height*1.7/2
        color: enabled? Style.currentTheme.accent: Style.currentTheme.background
        opacity: 0

    }


}
