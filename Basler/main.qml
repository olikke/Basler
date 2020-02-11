import QtQuick 2.11
import QtQuick.Window 2.2
import QtQuick.Controls 2.4
import QtMultimedia 5.9
import com.PylonFeaturesEnum 1.0
import QtQuick.Controls.Material 2.3
import QtQuick.Controls.Universal 2.3
import QtGraphicalEffects 1.0
import "."
import "qrc:/../QML/TM2221_5/"
import "qrc:/../QML/CustomControls/"
import "qrc:/../QML/TM2221_5/"
import "qrc:/../QML/Style/"
import "qrc:/BaslerControls"


Window {    
    id: window
    visible: true
    width: 1920
    height: 1080
    title: qsTr("Basler Pylon Example")
    flags: Qt.FramelessWindowHint
    visibility: Window.FullScreen
    Component.onCompleted: {
        Style.settheme = true
        Material.theme=Material.Dark
    }

    property var tm2221:unitsManager.getUnitPtr("TM-2221-5")
    onTm2221Changed: {
        if (tm2221==null) tm2221=undefined
        else tm2221.startParse(true)
    }


    property var em5215:tm2221.getEM5215()
    onEm5215Changed: {
        if (em5215==null)
        {
            em5215=undefined
            return
        }
    }

    property var myCamera: unitsManager.getCamera()

    onMyCameraChanged: {
        if (myCamera==null) myCamera=undefined
    }

    property var log: unitsManager.getLog()

    onLogChanged: {
        if (log==null) log=undefined
    }


    property bool controlEnabled: myCamera.opening
    property bool openEnabled: myCamera.pylonChoice!==-1
    property int oneElementWidth: 315
    property int oneElementHeight: 27
    property int buttonHeight: 37
    property int column1: 120
    property int column2: 80
    property int gap: 5
    property color mc
    property color mc2

    property var pn: myCamera.nameSpace
    property var mySurface:myCamera.abs

    Rectangle {
        id: caption
        height: 36
        color: Style.currentTheme.background
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.top: parent.top
        anchors.topMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        z: 1

        Rectangle {
            height: 34
            color: Style.currentTheme.primary
            anchors.bottomMargin: 2
            anchors.right: parent.right
            anchors.rightMargin: 0
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.bottom: parent.bottom
            anchors.top: parent.top
            anchors.topMargin: 0

            Button {
                width: 70
                height: parent.height
                text: qsTr("ВЫХОД")
                padding: 0
                highlighted: false
                hoverEnabled: true
                font.bold: true
                font.family: Style.robotoCondensed
                anchors.right: parent.right
                anchors.rightMargin: 5
                anchors.verticalCenter: parent.verticalCenter
                Material.elevation: 0
                Material.background: Style.currentTheme.background
                onClicked: Qt.quit()
            }

            Text {
                text: qsTr("TM-2221-5")
                font.family: Style.robotoCondensed
                font.pointSize: 14
                font.bold: true
                font.capitalization: Font.AllUppercase
                color:  Style.currentTheme.foreground
                anchors.verticalCenter: parent.verticalCenter
                anchors.verticalCenterOffset: 0
                anchors.left: parent.left
                anchors.leftMargin: 10
            }
        }
    }

    ListModel {
        id: cbItems
        ListElement { text: "Выкл" }
        ListElement { text: "Один раз" }
        ListElement { text: "Непрерывно" }
    }

    Rectangle {
        id: controlRect
        width: 384
        color: Style.currentTheme.background
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        antialiasing: true
        anchors.top: caption.bottom
        anchors.topMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0

        Rectangle {
            id: rectangle
            color: Style.currentTheme.primary
            anchors.rightMargin: 2
            anchors.fill: parent

            Column {
                anchors.fill: parent
                spacing: 2

                OpenBasler {
                    width: parent.width
                }

                Separator {
                    width: parent.width
                }

                AOI {
                    width: parent.width
                }

                Separator {
                    width: parent.width
                }

                Gain {
                    width: parent.width
                }

                Separator {
                    width: parent.width
                }

                Exposure {
                    width: parent.width
                }

                Separator {
                    width: parent.width
                }

                EM5215{
                    width: parent.width
                    height: 250
                    em5215: tm2221.getEM5215()

                }
            }
        }
    }

    Rectangle {
        id: videoRect
        color: Style.currentTheme.background
        anchors.leftMargin: 0
        anchors.top: caption.bottom
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.left: controlRect.right
        anchors.topMargin: 0

        Rectangle {
            id: rectangle4
            height: 864
            width:1536
            color: Style.currentTheme.primary
            anchors.top: parent.top
            anchors.topMargin: 0
            anchors.right: parent.right
            anchors.rightMargin: 0
            anchors.left: parent.left
            anchors.leftMargin: 0

            VideoOutput {
                id: videooutput
                anchors.fill: parent
                fillMode: VideoOutput.PreserveAspectFit
                source: mySurface
            }
        }

        Rectangle {
            id: rLogger
            color: Style.currentTheme.primary
            anchors.leftMargin: 0
            anchors.bottom: videoRect.bottom
            anchors.bottomMargin: 0
            anchors.top: rectangle4.bottom
            anchors.topMargin: 2
            anchors.right: parent.right
            anchors.rightMargin: 0
            anchors.left: parent.left
            CustomLabel {
                id: loggerCaption
                text: "Сообщения"
                color: Style.currentTheme.accent
                x: 10
                y: 5
            }


            ListView {
                id: listView
                anchors.top: loggerCaption.bottom
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.margins: 10
                anchors.topMargin: 5
                model: log.logger
                delegate: Rectangle {
                    width: parent.width
                    height: mytext3.height
                    color: rLogger.color
                    Text {
                        id: mytext3
                        color: Style.currentTheme.foreground
                        font.bold: true
                        font.family: Style.robotoCondensed
                        font.pointSize: 10
                        font.capitalization: Font.AllUppercase
                        clip: true
                        text: modelData
                    }
                }
            }
        }
    }
}
