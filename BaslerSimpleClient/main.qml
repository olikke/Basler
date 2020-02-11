import QtQuick 2.11
import QtQuick.Window 2.2
import QtQuick.Controls 2.4
import QtGraphicalEffects 1.0
import QtQuick.Controls.Material 2.3
import QtMultimedia 5.8
import "."
import "qrc:/../QML/TM2221_5/"
import "qrc:/../QML/CustomControls/"
import "qrc:/../QML/TM2221_5/"
import "qrc:/../QML/Style/"


Window {
    id: window
    visible: true
    width: 960
    height: 765
    flags: Qt.FramelessWindowHint
    color: Style.currentTheme.background
    Component.onCompleted: {
        Style.settheme = true
        Material.theme=Material.Dark
        setX(Screen.width/2-width/2)
        setY(Screen.height/2-height/2)
    }

    property var tm2221:unitsManager.getUnitPtr("TM-2221-5")
    onTm2221Changed: {
        if (tm2221==null) tm2221=undefined
    }

    property var log: unitsManager.getLog()

    property var mySurface:controller.abs
    property color mc
    property int previousX
    property int previousY
    property bool shown: false
    property bool fullscreen:false



    Rectangle {
        id: caption
        height: 36
        z: 1
        color: Style.currentTheme.background
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.top: parent.top
        anchors.topMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        objectName: "caption"

        Rectangle {
            id: caption2
            color: Style.currentTheme.primary
            anchors.right: parent.right
            anchors.rightMargin: 0
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.bottom: parent.bottom
            anchors.bottomMargin:2
            anchors.top: parent.top
            anchors.topMargin: 0

            MouseArea {
                width: 40
                height: 40
                anchors.left: parent.left
                anchors.leftMargin: 5
                anchors.verticalCenter: parent.verticalCenter
                anchors.verticalCenterOffset: 0
                hoverEnabled: true
                onEntered: mc=Style.currentTheme.accent
                onExited: mc=Style.currentTheme.background
                onPressed:{
                    shown=!shown
                    animation.running= true
                    animation2.running = true
                }
            }
            Image {
                id: im
                anchors.left: parent.left
                anchors.leftMargin: 5
                anchors.verticalCenter: parent.verticalCenter
                anchors.verticalCenterOffset: 0
                source: "../resources/icons/outline-menu-24px.svg"
            }
            ColorOverlay {
                anchors.fill: im
                source: im
                color: mc
            }
            Text {
                id: text1
                text: qsTr("TM-2221-5 клиент")
                anchors.verticalCenter: parent.verticalCenter
                anchors.verticalCenterOffset: 0
                anchors.left: parent.left
                anchors.leftMargin: 35
                font.family: Style.robotoCondensed
                font.pointSize: 14
                font.bold: true
                font.capitalization: Font.AllUppercase
                color:  Style.currentTheme.foreground
            }
            Rectangle {
                property int temp: tm2221.state
                onTempChanged: {
                    switch (temp){
                    case 0: color=Style.unitGreen; break
                    case 1: color=Style.unitYellow; break
                    case 2: color=Style.unitRed; break
                    }
                }
                width: 20
                height: 20
                radius: Style.unitRadius/2
                anchors.left: text1.right
                anchors.leftMargin: 10
                anchors.verticalCenter: parent.verticalCenter
            }

            Button {
                id: button
                width: 70
                height: parent.height
                text: qsTr("ВЫХОД")
                rightPadding: 0
                leftPadding: 0
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
        }
    }

    Rectangle {
        id: video
        color: Style.currentTheme.primary
        anchors.topMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.top: caption.bottom
        MouseArea {
            id: mouseAreaMoveApp
            anchors.topMargin: 0
            anchors.fill: parent
            onPressed: {
                previousX=mouseX
                previousY=mouseY
            }
            onMouseXChanged: {
                var dx = mouseX - previousX
                window.setX(window.x + dx)
            }
            onMouseYChanged: {
                var dy = mouseY - previousY
                window.setY(window.y + dy)
            }
            onDoubleClicked: {
                fullscreen? window.showNormal(): window.showFullScreen()

                fullscreen=!fullscreen
            }
        }
        VideoOutput {
            id: videooutput
            width: 0
            height: 0
            anchors.bottom: justRec.top
            anchors.bottomMargin: 0
            anchors.right: parent.right
            anchors.rightMargin: 0
            anchors.left: parent.left
            anchors.top: parent.top
            fillMode: VideoOutput.PreserveAspectFit
            source: mySurface
        }

        Rectangle {
            id: justRec
            height: fullscreen? 0 : 189
            color: Style.currentTheme.background
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            anchors.right: parent.right
            anchors.left: parent.left

            Rectangle {
                id: logRectangle
                color:Style.currentTheme.primary
                x: 0
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 0
                anchors.topMargin: 2
                anchors.top: parent.top
                CustomLabel {
                    id: loggerCaption
                    text: "Сообщения"
                    x: 5
                    y: 5
                }

                NumberAnimation on x
                {
                    id: animation2
                    running: false
                    from: shown? 0 : controlPanel.width
                    to: shown? controlPanel.width: 0
                    duration: Style.animationDuration
                }

                Rectangle {
                    anchors.top: loggerCaption.bottom
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                    anchors.left: parent.left
                    anchors.margins: 10
                    anchors.topMargin: 5
                    color: Style.currentTheme.background
                    radius: Style.unitRadius/2
                    anchors.rightMargin: 10

                    ListView {
                        id: listView
                        anchors.fill: parent
                        anchors.margins: 5
                        model: log.logger
                        clip: true
                        delegate: Rectangle {
                            width: parent.width
                            height: mytext3.height
                            color:Style.currentTheme.background
                            Text {
                                id: mytext3
                                color: Style.currentTheme.foreground
                                text: modelData
                                font.family: Style.robotoCondensed
                                font.pixelSize: 12
                                font.capitalization: Font.AllUppercase
                            }
                        }
                    }
                }
            }
        }
    }

    Rectangle {
        id: controlPanel
        x: -width
        width: 400
        color: Style.currentTheme.background
        anchors.bottom: video.bottom
        anchors.bottomMargin: 0
        anchors.top: video.top
        anchors.topMargin: 0

        NumberAnimation on x
        {
            id: animation
            running: false
            from: shown? -400:0
            to:shown? 0:-400
            duration: Style.animationDuration
        }

        Flickable {
            id: flickable
            anchors.fill: controlPanel
            contentHeight: rectangle.height

            Rectangle {
                id: rectangle
                height: 910
                width: parent.width-2
                color:Style.currentTheme.primary

                MainTM2221{
                    width: parent.width
                    height: parent.height

                }

            }
        }

    }

}
