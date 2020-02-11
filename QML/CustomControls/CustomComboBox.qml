import QtQuick 2.11
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.3

import "qrc:/../QML/Style/"

ComboBox {
    id: control
    clip: true
    model: ["First", "Second", "Third"]
    font.family: Style.robotoCondensed
    font.pointSize: 10
    font.bold: true
    font.capitalization: Font.AllUppercase
    onEnabledChanged: paint()


    function paint()
    {
        canvas.requestPaint()
    }

    delegate: ItemDelegate {
        width: control.width
        height: 27
        contentItem: Text {

            text: modelData
            color: Style.currentTheme.foreground
            font: control.font
            elide: Text.ElideRight
            verticalAlignment: Text.AlignVCenter
            anchors.left: parent.left
            anchors.leftMargin: 5
        }
        highlighted: control.highlightedIndex === index
        //тут поставить цвет выделения
        background: Rectangle {
            color: Style.currentTheme.primary
            anchors.fill: parent

        }
    }

    indicator: Canvas {
        id: canvas
        x: control.width - width - control.rightPadding
        y: control.topPadding + (control.availableHeight - height) / 2
        width: 12
        height: 8
        contextType: "2d"

        Connections {
            target: control
            onPressedChanged: canvas.requestPaint()
        }

        onPaint: {
            context.reset()
            context.moveTo(0, 0)
            context.lineTo(width, 0)
            context.lineTo(width / 2, height)
            context.closePath()
            context.fillStyle = control.enabled ? Style.currentTheme.foreground : Qt.lighter(Style.currentTheme.primary,1.4)
            context.fill()
        }
    }

    contentItem: Text {
        leftPadding: 0
        rightPadding: control.indicator.width + control.spacing
        text: control.displayText
        font: control.font
        color: control.enabled? Style.currentTheme.foreground : Qt.lighter(Style.currentTheme.primary,1.4)
        verticalAlignment: Text.AlignVCenter
        anchors.left: parent.left
        anchors.leftMargin: 5
    }

    background: Rectangle {
        implicitWidth: 110
        implicitHeight: 25
        radius: Style.unitRadius/2
        color: Style.currentTheme.primary
    }

    popup: Popup {
        y: control.height - 1
        width: control.width
        implicitHeight: contentItem.implicitHeight
        padding:1

        contentItem: ListView {
            spacing: 5
            clip: true
            implicitHeight: contentHeight
            model: control.popup.visible ? control.delegateModel : null
            currentIndex: control.highlightedIndex
            ScrollIndicator.vertical: ScrollIndicator { }
        }

        background: Rectangle {
            border.color: Style.currentTheme.primary
            color: Style.currentTheme.primary
            radius: 2
        }
    }
}
