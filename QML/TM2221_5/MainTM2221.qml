import QtQuick 2.11
import QtQuick.Controls 2.2

import "qrc:/../QML/ControlsSet/"
import "qrc:/../QML/CustomControls/"

Item {

//    Component.onCompleted: {
//        tm2221.startParse(true)
//    }
//    Component.onDestruction: {
//        tm2221.startParse(false)
//    }

    CustomLabel {
        text: "Нет обмена данными"
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        font.pointSize: 20
        visible: !tm2221.dataExh
    }

    Column {
        id: column
        anchors.fill: parent
        spacing: 5
        visible:  tm2221.dataExh

        Resolution{
        }

        Separator{
            width: parent.width
        }

        KuMode {
            videoServer: tm2221.getVideoServer()
        }

        Separator{
            width: parent.width
        }

        VideoServer {
            id: videoserver
            width: parent.width
            videoServer: tm2221.getVideoServer()
        }

        Separator{
            width: parent.width
        }

        EM5215 {
            width: parent.width
            height: 250
            em5215: tm2221.getEM5215();
        }
    }

}
