import QtQuick 2.11
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.3

import "qrc:/../QML/ControlsSet/"
import "qrc:/../QML/CustomControls/"
import "qrc:/../QML/Style/"

Item {

    height: column.openHeight

    property int oneElementHeight: 20

    function cameraChange()
    {
        myCamera.choice=pylonDevices.curValue
    }

    function cameraOpen()
    {
        myCamera.open(!myCamera.opening)
    }

    function cameraEnum()
    {
        myCamera.enumCameras()
    }

    function cameraPlay()
    {
        myCamera.startVideo(!myCamera.playing)
    }

    function loadFeatures()
    {
        myCamera.loadFeatures(loadSettings.curValue)
    }

    function saveFeatures()
    {
        myCamera.saveFeatures(saveSettings.curValue)
    }

    Column {
        id: column
        anchors.topMargin: 0
        anchors.margins: 10
        anchors.fill: parent
        spacing: 2

        property int openHeight: column2.implicitHeight+caption.height+column.spacing+2
        property int collapseHeight :caption.height+column.spacing+5

        CustomLabel {
            id: caption
            text: "Управление"
            color: Style.currentTheme.accent
        }

        Column {
            id: column2
            width: parent.width
            spacing: 2

            ComboBoxPanel {
                id: pylonDevices
                panelName: "Выбор камеры"
                width: parent.width
                height: oneElementHeight
                customModel: myCamera.devices
                updateValueFunction: cameraChange
            }

            Row {
                spacing: 2
                width: parent.width
                height: oneElementHeight
                ButtonNoEnabled{
                    enabledd: myCamera.choice!==-1
                    name: controlEnabled? "Закрыть":"Открыть"
                    width: parent.width/3-1
                    height: oneElementHeight
                    link: cameraOpen

                }

                ButtonNoEnabled{
                    enabledd: controlEnabled
                    width: parent.width/3-1
                    height: oneElementHeight
                    name: myCamera.playing? "Стоп" : "Старт"
                    link: cameraPlay
                }

                ButtonNoEnabled{
                    enabledd: true
                    name: "Обновить"
                    width: parent.width/3
                    height: oneElementHeight
                    link: cameraEnum
                }
            }



            ComboBoxPanel {
                id: loadSettings
                panelName: "Загрузить"
                width: parent.width
                height: oneElementHeight
                customModel: ["1","2","3"]
                updateValueFunction: loadFeatures
            }

            ComboBoxPanel {
                id: saveSettings
                panelName: "Сохранить"
                width: parent.width
                height: oneElementHeight
                customModel: ["1","2","3"]
                updateValueFunction: saveFeatures
            }
        }
    }
}
