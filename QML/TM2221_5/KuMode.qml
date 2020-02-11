import QtQuick 2.11
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.3

import "qrc:/../QML/ControlsSet/"
import "qrc:/../QML/CustomControls/"
import "qrc:/../QML/Style/"

Item {
    width: parent.width
    height: column.implicitHeight+5
    property var videoServer: null

    Column {
        id: column
        anchors.topMargin: 0
        anchors.margins: 10
        anchors.fill: parent
        spacing: 5

        CustomLabel {
            text: "Режим КУ"
            //color: Style.currentTheme.accent
        }

        SwitchTreeItem{
            property int temp: videoServer.mode
            onTempChanged: index=temp
            width: parent.width
            textLeft: "1"
            textMiddle: "2"
            textRight: "3"
            height: 20
            index: temp
            onIndexChanged: if (index!==temp) tm2221.changeKuMode(index)
        }
    }
}
