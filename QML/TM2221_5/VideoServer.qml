import QtQuick 2.0
import QtQuick.Controls 2.2

import "qrc:/../QML/Style/"
import "qrc:/../QML/ControlsSet/"
import "qrc:/../QML/CustomControls/"

Item {
    id: root
    clip: true
    height: column.implicitHeight+column.anchors.margins

    property var videoServer: null

    property int oneElementHeight: 20

    property bool de: videoServer.dataExh

    Column {
        id: column
        anchors.topMargin: 0
        anchors.margins: 10
        anchors.fill: parent
        spacing: 5

        CustomLabel {
            text: "Видеокамера сетевая (А6)"
           // color: Style.currentTheme.accent
        }

        Column {
            width: parent.width
            spacing: 2

            TextPanel {
                panelName: "Обмен данными"
                panelValue: de? "есть": "нет"
                stateColor: de? Style.unitGreen : Style.unitRed
                width: parent.width
                height: oneElementHeight
            }

            TextPanel {
                panelName: "Серийный номер"
                panelValue: videoServer.serialNumber.toString()
                width: parent.width
                height: oneElementHeight
                panelEnabled: de
            }

            TextPanel {
                property real temp: videoServer.temperature
                onTempChanged: {
                    switch (videoServer.getTemperatureColor()){
                    case 0: stateColor=Style.unitGreen; break
                    case 1: stateColor=Style.unitYellow; break
                    case 2: stateColor=Style.unitRed; break
                    }
                }
                panelName: "Температура платы, C°"
                panelValue: temp.toFixed(2)
                width: parent.width
                height: oneElementHeight
                panelEnabled: de
            }


            TextPanel {
                panelName: "Получено кадров"
                height: oneElementHeight
                panelValue: videoServer.totalFrame
                panelEnabled: de
            }

            TextPanel {
                panelName: "Кадров/сек"
                height: oneElementHeight
                panelValue: videoServer.framePerSec.toFixed(2)
                panelEnabled: de
            }

            TextPanel{
                panelName: "Кадров с ошибками"
                height: oneElementHeight
                panelValue: videoServer.failedFrame
                panelEnabled: de
            }
        }

        Separator{
            width: parent.width+20
            anchors.horizontalCenter: parent.horizontalCenter
        }

        CustomLabel {
            text: "Основной сервер"
            font.capitalization: Font.AllUppercase
           // color: Style.currentTheme.accent
        }

        Column {
            width: parent.width
            spacing: 2

            TextPanel {
                id: fullTotalFrame
                panelName: "Отправлено кадров"
                height: oneElementHeight
                panelValue: videoServer.fullTotalFrame
                panelEnabled: de
            }

            TextPanel {
                id: fullFramePerSec
                panelName: "Кадров/сек"
                height: oneElementHeight
                panelValue: videoServer.fullFramePerSec.toFixed(2)
                panelEnabled: de
            }

            TextPanel{
                panelName: "МБайт/сек"
                height: oneElementHeight
                panelValue: videoServer.fullMBPerSec.toFixed(2)
                panelEnabled: de
            }
        }

        Separator{
            width: parent.width+20
            anchors.horizontalCenter: parent.horizontalCenter
        }

        CustomLabel {
            text: "Сервер предпросмотра"
            font.capitalization: Font.AllUppercase
           // color: Style.currentTheme.accent
        }

        Column {
            width: parent.width
            spacing: 2

            TextPanel {
                panelName: "Отправлено кадров"
                height: oneElementHeight
                panelValue: fullTotalFrame.panelValue
                panelEnabled: de
            }

            TextPanel {
                panelName: "Кадров/сек"
                height: oneElementHeight
                panelValue: fullFramePerSec.panelValue
                panelEnabled: de
            }

            TextPanel{
                panelName: "МБайт/сек"
                height: oneElementHeight
                panelValue: videoServer.halfMBPerSec.toFixed(2)
                panelEnabled: de
            }
        }
    }
}
