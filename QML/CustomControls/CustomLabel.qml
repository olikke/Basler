import QtQuick 2.11

import "qrc:/../QML/Style/"

Text {
    clip: true
    font.family: Style.robotoCondensed
    font.pointSize: 10
    color: Style.currentTheme.foreground
    font.bold: true
    verticalAlignment: Text.AlignVCenter
    horizontalAlignment: Text.AlignHCenter
    font.capitalization: Font.AllUppercase
}
