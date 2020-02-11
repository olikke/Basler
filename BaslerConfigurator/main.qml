import QtQuick 2.11
import QtQuick.Window 2.2
import QtQuick.Controls 2.4
import QtGraphicalEffects 1.0
import QtQuick.Controls.Material 2.3
import com.Settings 1.0
import "."



Window {
    visible: true
    width: 500
    height: 475
    flags: Qt.FramelessWindowHint
    id: mainWindow
    color: Style.background
    Component.onCompleted: {
        Style.settheme = true
        Material.theme=Material.Dark
        setX(Screen.width/2-width/2)
        setY(Screen.height/2-height/2)
    }

    property color mc
    property color mc2
    property int previousX
    property int previousY
    property bool broadcast: false

    property string _broadcastIP
    property string _broadcastPort
    property string _fullPort
    property string _fullIP
    property string _fullQuality

    property string _halfPort
    property string _halfIP
    property string _halfQuality

    property string _controlIP
    property string _controlPort

    //    property bool _needGUI



    Settings{
        id:settings
        Component.onCompleted: notfound.visible=!settings.openFile()
        onReadyChanged: {
            notfound.visible=!ready
            if (ready)
            {
                var Array = settings.allValue()
                _fullPort=Array[0]
                _fullIP=Array[1]
                _fullQuality=Array[2]

                fullPort.text=_fullPort
                fullIP.text=_fullIP
                fullQuality.text=_fullQuality

                _halfPort=Array[3]
                _halfIP=Array[4]
                _halfQuality=Array[5]

                halfPort.text=_halfPort
                halfIP.text=_halfIP
                halfQuality.text=_halfQuality

                _controlIP=Array[6]
                _controlPort=Array[7]
                _broadcastIP=Array[8]

                controlIP.text=_controlIP;
                controlPort.text=_controlPort
                broadcastIP.text=_broadcastIP
            }
        }
        onNeedGUIChanged: needGUI.checked
        onMessages: {
            message.text=mess
            goodNews? message.color=Qt.darker(Style.foreground,2.9) : message.color=Style.dangerous
        }
    }

    Rectangle {
        id: rectangle2
        height: 40
        color: Style.background
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.top: parent.top
        anchors.topMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0

        Rectangle {
            id: rectangle3
            color: Style.primary
            anchors.right: parent.right
            anchors.rightMargin: 0
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 5
            anchors.top: parent.top
            anchors.topMargin: 0
            MouseArea {
                id: ma1
                width: 40
                height: 40
                anchors.left: parent.left
                anchors.leftMargin: 5
                y: 0
                hoverEnabled: true
                onEntered: mc2=Style.accent
                onExited: mc2=Style.background
            }
            Image {
                id: im
                x: 5
                y: 5
                source: "../resources/icons/outline-pets-24px.svg"
            }
            ColorOverlay {
                id: co1
                anchors.fill: im
                source: im
                color: mc2
            }
            MouseArea {
                id: ma2
                width: 40
                height: 40
                anchors.right: parent.right
                anchors.rightMargin: 5
                y: 0
                hoverEnabled: true
                onEntered: mc=Style.accent
                onExited: mc=Style.background
                onClicked: Qt.quit()
            }
            Image {
                id: imageclose
                anchors.right: parent.right
                anchors.rightMargin: 5
                y: 5
                source: "../resources/icons/outline-clear-24px.svg"
            }
            ColorOverlay {
                id: co2
                anchors.fill: imageclose
                source: imageclose
                color: mc
            }

            Text {
                id: text1
                x: 35
                y: 8
                text: qsTr("Конфигуратор КУ")
                font.family: Style.regularFont.name
                font.pixelSize: Style.fontSizeBig*1.2
                color:  Style.foreground
            }

            MouseArea {
                id: ma3
                anchors.rightMargin: 0
                anchors.leftMargin: 0
                anchors.right: ma2.left
                anchors.top: parent.top
                anchors.left: ma1.right
                anchors.bottom: parent.bottom
                anchors.topMargin: 0
                onPressed: {
                    previousX=mouseX
                    previousY=mouseY
                }
                onMouseXChanged: {
                    var dx = mouseX - previousX
                    mainWindow.setX(mainWindow.x + dx)
                }
                onMouseYChanged: {
                    var dy = mouseY - previousY
                    mainWindow.setY(mainWindow.y + dy)
                }
            }
        }
    }

    MouseArea {
        id: mouseArea
        anchors.top: rectangle2.bottom
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.topMargin: 0
        onPressed: {
            previousX=mouseX
            previousY=mouseY
        }
        onMouseXChanged: {
            var dx = mouseX - previousX
            mainWindow.setX(mainWindow.x + dx)
        }
        onMouseYChanged: {
            var dy = mouseY - previousY
            mainWindow.setY(mainWindow.y + dy)
        }
    }

    Rectangle {
        id: sett
        color: Style.background
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        antialiasing: true
        anchors.top: rectangle2.bottom
        anchors.topMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0

        Rectangle {
            id: sett2
            color: Style.background
            radius: Style.unite_cover_radius
            border.color: Style.primary
            border.width: 2
            anchors.right: parent.right
            anchors.rightMargin: 5
            anchors.bottom: parent.bottom
            antialiasing: true
            anchors.top: parent.top
            anchors.topMargin: 0
            anchors.left: parent.left
            anchors.leftMargin: 5
            anchors.bottomMargin: 48

            Rectangle {
                id: fullServer
                height: 80
                color: Style.background
                radius: Style.unite_cover_radius
                opacity: mode.checked? 1 : 0.2
                anchors.leftMargin: 12
                anchors.rightMargin: 8
                anchors.right: parent.right
                anchors.topMargin: 14
                anchors.left: parent.left
                border.color: Style.primary
                border.width: 2
                antialiasing: true
                anchors.top: text19.bottom
                enabled: mode.checked

                Text {
                    id: full1
                    x: 5
                    y: -25
                    width: 178
                    height: 17
                    color: Style.foreground
                    text: qsTr("СЕРВЕР ОСНОВНОЙ")
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                    anchors.top: parent.top
                    anchors.topMargin: 10
                    font.family: Style.regularFont.name
                    font.pixelSize: 16
                }

                Text {
                    id: full2
                    width: 88
                    height: 17
                    text: qsTr("Адрес")
                    anchors.left: parent.left
                    anchors.leftMargin: 15
                    anchors.top: full1.bottom
                    anchors.topMargin: 5
                    font.family: Style.regularFont.name
                    font.pixelSize: 16
                    color:Style.accent
                }

                Text {
                    id: full3
                    width: 178
                    height: 17
                    text: qsTr("Качество")
                    anchors.left: parent.left
                    anchors.leftMargin: 15
                    anchors.top: full2.bottom
                    anchors.topMargin: 5
                    font.family: Style.regularFont.name
                    font.pixelSize: 16
                    color: Style.accent
                }

                TextInput {
                    id: fullPort
                    x: 638
                    y: -4
                    width: fullPort.contentWidth
                    height: fullPort.contentHeight
                    anchors.top: full2.top
                    anchors.topMargin: 0
                    anchors.right: fullServer.right
                    anchors.rightMargin: 5
                    font.family: Style.regularFont.name
                    font.pixelSize: 16
                    color: text===_fullPort? Style.foreground: Style.dangerous
                    text: "port"
                    horizontalAlignment: Text.AlignRight
                    validator: IntValidator {bottom: 1025; top: 9000}
                }

                TextInput {
                    id: fullIP
                    x: 597
                    y: -4
                    width: 31
                    height: 20
                    color: text===_fullIP? Style.foreground: Style.dangerous
                    text: "ipipipip"
                    anchors.right: fullPort.left
                    font.family: Style.regularFont.name
                    anchors.topMargin: 0
                    validator: RegExpValidator{regExp:  /^((?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])\.){0,3}(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])$/}
                    anchors.rightMargin: 10
                    font.pixelSize: 16
                    anchors.top: full2.top
                }

                TextInput {
                    id: fullQuality
                    x: 634
                    width: 31
                    height: 20
                    color: text===_fullQuality? Style.foreground: Style.dangerous
                    text: "65"
                    anchors.right: fullPort.right
                    font.family: Style.regularFont.name
                    anchors.top: full3.top
                    anchors.topMargin: 0
                    validator: IntValidator {
                        bottom: 1
                        top: 100
                    }
                    horizontalAlignment: Text.AlignRight
                    anchors.rightMargin: 0
                    font.pixelSize: 16
                }
            }

            Text {
                id: text19
                x: 15
                width: 178
                height: 17
                color: Style.accent
                text: qsTr("Режим просмотра")
                anchors.top: parent.top
                anchors.topMargin: 27
                font.pixelSize: 16
                font.family: Style.regularFont.name
            }



            Rectangle {
                id: halfServer
                height: 80
                color: Style.background
                radius: Style.unite_cover_radius
                anchors.right: parent.right
                anchors.rightMargin: 10
                anchors.left: parent.left
                anchors.leftMargin: 10
                anchors.topMargin: 10
                border.color: Style.primary
                border.width: 2
                antialiasing: true
                anchors.top: fullServer.bottom
                opacity: mode.checked? 1: 0.2
                enabled: mode.checked

                Text {
                    id: half1
                    width: 178
                    height: 17
                    color: Style.foreground
                    text: qsTr("СЕРВЕР ПРЕДПРОСМОТРА")
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                    anchors.top: parent.top
                    anchors.topMargin: 10
                    font.family: Style.regularFont.name
                    font.pixelSize: 16
                }

                Text {
                    id: half2
                    width: 178
                    height: 17
                    color: Style.accent
                    text: qsTr("Адрес")
                    anchors.left: parent.left
                    anchors.leftMargin: 15
                    anchors.top: half1.bottom
                    anchors.topMargin: 5
                    font.family: Style.regularFont.name
                    font.pixelSize: 16
                }

                Text {
                    id: half3
                    width: 178
                    height: 17
                    color: Style.accent
                    text: qsTr("Качество")
                    anchors.left: parent.left
                    anchors.leftMargin: 15
                    anchors.top: half2.bottom
                    anchors.topMargin: 5
                    font.family: Style.regularFont.name
                    font.pixelSize: 16


                }

                TextInput {
                    id: halfIP
                    x: 393
                    y: -113
                    width: 31
                    height: 20
                    color: text===_halfIP? Style.foreground: Style.dangerous
                    text: "ipipip"
                    anchors.right: halfPort.left
                    anchors.rightMargin: 10
                    font.family: Style.regularFont.name
                    anchors.topMargin: 0
                    validator: RegExpValidator {
                        regExp: /^((?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])\.){0,3}(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])$/
                    }
                    font.pixelSize: 16
                    anchors.top: half2.top
                }

                TextInput {
                    id: halfQuality
                    x: 434
                    y: 59
                    width: 31
                    height: 20
                    color: text===_halfQuality? Style.foreground : Style.dangerous
                    text: "65"
                    anchors.right: halfServer.right
                    font.family: Style.regularFont.name
                    anchors.topMargin: 0
                    validator: IntValidator {
                        bottom: 1
                        top: 100
                    }
                    horizontalAlignment: Text.AlignRight
                    anchors.rightMargin: 5
                    font.pixelSize: 16
                    anchors.top: half3.top
                }

                TextInput {
                    id: halfPort
                    x: 434
                    y: -113
                    width: 31
                    height: 20
                    color: text===_halfPort? Style.foreground: Style.dangerous
                    text: "port"
                    anchors.right: halfServer.right
                    font.family: Style.regularFont.name
                    anchors.top: half2.top
                    anchors.topMargin: 0
                    validator: IntValidator {
                        bottom: 1025
                        top: 9000
                    }
                    horizontalAlignment: Text.AlignRight
                    anchors.rightMargin: 5
                    font.pixelSize: 16
                }


            }

            Rectangle {
                id: control
                height: 80
                color: Style.background
                radius: Style.unite_cover_radius
                anchors.right: parent.right
                anchors.rightMargin: 10
                anchors.left: parent.left
                anchors.leftMargin: 10
                anchors.topMargin: 10
                border.color: Style.primary
                border.width: 2
                antialiasing: true
                anchors.top: halfServer.bottom

                Text {
                    id: control1
                    width: 178
                    height: 17
                    color: Style.foreground
                    text: qsTr("УПРАВЛЕНИЕ / ДИАГНОСТИКА")
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                    anchors.top: parent.top
                    anchors.topMargin: 10
                    font.family: Style.regularFont.name
                    font.pixelSize: 16
                }

                Text {
                    id: text16
                    x: 5
                    y: -198
                    width: 178
                    height: 17
                    color: Style.accent
                    text: qsTr("Адрес свой")
                    font.family: Style.regularFont.name
                    font.pixelSize: 16
                }

                Text {
                    id: text18
                    x: 5
                    y: -411
                    width: text18.contentWidth
                    height: 17
                    color: Style.accent
                    text: qsTr("Рассылка всем")
                    font.family: Style.regularFont.name
                    font.pixelSize: 16
                }

                CheckBox {
                    id: checkBox
                    x: 428
                    y: -427
                    text: qsTr("")
                    anchors.right: parent.right
                    anchors.rightMargin: 10
                    anchors.verticalCenter: text18.verticalCenter
                    Material.accent: Style.accent
                    checked: broadcast=checkBox.checked
                }
            }



            Text {
                id: text17
                x: 15
                y: text18.y+25
                width: 178
                height: 17
                color: broadcast?  Qt.lighter(Style.background,1.4): Style.accent
                text: qsTr("Рассылка адрес")
                font.family: Style.regularFont.name
                font.pixelSize: 16
            }

            Button {
                id: button
                y: text17.y+25
                height: 37
                text: qsTr("Сохранить")
                anchors.right: parent.right
                anchors.rightMargin: 10
                anchors.left: parent.left
                anchors.leftMargin: 10
                onClicked: {
                    if (broadcast)
                    {
                        broadcastIP.text=settings.findBroadcast(controlIP.text)
                    }
                    var arr = [
                                fullPort.text,
                                fullIP.text,
                                fullQuality.text,

                                halfPort.text,
                                halfIP.text,
                                halfQuality.text,

                                controlIP.text,
                                controlPort.text,
                                broadcastIP.text]
                    settings.allUpdate(arr)
                }
            }

            Button {
                id: button1
                y: button.y+30
                height: 37
                text: qsTr("Отменить")
                anchors.right: parent.right
                anchors.left: parent.left
                anchors.rightMargin: 10
                anchors.leftMargin: 10
                onClicked: settings.loadDefault()
            }



            TextInput {
                id: controlIP
                width: 31
                height: 20
                color: text===_controlIP? Style.foreground: Style.dangerous
                text: _controlIP
                anchors.right: controlPort.left
                anchors.rightMargin: 10
                font.family: Style.regularFont.name
                anchors.topMargin: 0
                validator: RegExpValidator {
                    regExp: /^((?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])\.){0,3}(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])$/
                }
                font.pixelSize: 16
                anchors.top: text16.top
            }

            TextInput {
                id: controlPort
                width: 31
                height: 20
                color: text===_controlPort? Style.foreground: Style.dangerous
                text: _controlPort
                anchors.right: halfServer.right
                font.family: Style.regularFont.name
                anchors.top: text16.top
                anchors.topMargin: 0
                validator: IntValidator {
                    bottom: 1025
                    top: 9000
                }
                horizontalAlignment: Text.AlignRight
                anchors.rightMargin: 5
                font.pixelSize: 16
            }



            TextInput {
                id: broadcastIP
                enabled: !broadcast
                width: contentWidth
                height: 20
                color: broadcast?  Qt.lighter(Style.background,1.4): Style.foreground
                text: _broadcastIP
                anchors.top: text17.top
                anchors.topMargin: 3
                rightPadding: 5
                leftPadding: 5
                bottomPadding: 5
                topPadding: 5
                font.family:Style.regularFont.name
                anchors.right: fullServer.right
                validator: RegExpValidator {
                    regExp: /^((?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])\.){0,3}(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])$/
                }
                anchors.rightMargin: 5
                font.pixelSize: 16
            }

            Text {
                id: text20
                x: 4
                y: 6
                width: 178
                height: 17
                color: Style.accent
                text: qsTr("Режим сервера")
                anchors.verticalCenterOffset: 1
                anchors.verticalCenter: text19.verticalCenter
                anchors.left: parent.left
                font.pixelSize: 16
                font.family: Style.regularFont.name
                anchors.leftMargin: 229
            }

            Switch {
                id: mode
                x: 161
                y: 12
                height: 48
                text: qsTr("")
                anchors.verticalCenter: text19.verticalCenter
                font.family: "Times New Roman"
                autoExclusive: false
                checked: false
                Material.accent: Style.accent
            }



        }


        Rectangle {
            id: rectangle
            color: Style.background
            radius: Style.unite_cover_radius
            anchors.right: parent.right
            anchors.rightMargin: 5
            anchors.left: parent.left
            anchors.leftMargin: 5
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 5
            anchors.top: sett2.bottom
            anchors.topMargin: 5
            border.color: Style.primary
            border.width: 2

            Text {
                id: message
                x: 8
                y: 11
                font.pixelSize: 14
                font.family: Style.regularFont.name
            }
        }
    }

    Rectangle {
        id: notfound
        color: Style.background
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        antialiasing: true
        anchors.top: rectangle2.bottom
        anchors.topMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0

        Text {
            id: text2
            text: qsTr("Файл настроек не найден.")
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.right: parent.right
            anchors.rightMargin: 0
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignBottom
            anchors.top: parent.top
            anchors.topMargin: 0
            anchors.bottom: parent.bottom
            anchors.bottomMargin: parent.height/2
            font.pixelSize: 18
            font.family: Style.regularFont.name
            color: Style.dangerous
        }
        Text {
            id: text3
            text: qsTr("Загрузить настройки по умолчанию?")
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.right: parent.right
            anchors.rightMargin: 0
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignTop
            anchors.top: text2.bottom
            anchors.topMargin: 0
            font.pixelSize: 14
            font.family: Style.regularFont.name
            color: Style.dangerous
        }
        Button {
            id: yes
            height: 40
            width: 80
            anchors.top: text3.bottom
            anchors.topMargin: 10
            anchors.right: parent.right
            anchors.rightMargin: parent.width/2+10
            text: qsTr("ДА")
            onClicked: settings.loadDefault()
        }

        Button {
            id: no
            height: 40
            width: 80
            anchors.top: text3.bottom
            anchors.topMargin: 10
            anchors.left: parent.left
            anchors.leftMargin: parent.width/2+10
            text: qsTr("НЕТ")
            onClicked: Qt.quit()
        }
    }




}
