pragma Singleton
import QtQuick 2.0
import "."



QtObject{
    id:root

    property bool isDarkMode: true
    function setDarkMode(param){
        isDarkMode = param;
        currentTheme =  param? darkTheme : lightTheme;
    }

    property var currentTheme: darkTheme

    property var darkTheme:{
        "background": "#212121",
        "primary": "#616161",
        "accent": "#2196f3",
        "foreground": "#eeeeee"
    }

    property var lightTheme:{
        "background": "#fafafa",
        "primary": "#e0e0e0",
        "accent": "#2196f3",
        "foreground": "#424242"
    }

    property real fadeRectangleOpacity:0.92

    // FONTS
    property string robotoCondensed: regularFont.name
    property FontLoader regularFont: FontLoader { source: "qrc:/../resources/fonts/RobotoCondensed-Regular.ttf" }

    // COLORS

    // цвет шрифтов не зависящих от темы
    property color socketForeground: darkTheme.background
    // состояние блока: ОШИБКИ (работать нельзя)
    property color unitRed: "#f44336"
    // состояние блока: НЕТ ОШИБОК
    property color unitGreen: "#00E676"
    // состояние блока: ЕСТЬ ЗАМЕЧАНИЯ (не влияют на работу)
    property color unitYellow: "#FDD835"

    property color _OPTIC_Color: "#ff9800"
    property color _ETH_Color: "#00b0ff"
    property color _RS_Color: "#9c27b0"
    property color _ANALOG_Color: "#303f9f"
    property color _MPI_Color: "#ff5722"
    property color _SYNC_Color: "#0277bd"
    property color _YVIDEO_Color: "#e91e63"
    property color _SVIDEO_Color: "#d500f9"

    // NUMERIC VARIABLES
    property int animationDuration: 200
    property int unitRadius: 6
    property int connectionWidth: 2
}
