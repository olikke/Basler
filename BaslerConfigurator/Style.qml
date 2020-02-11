pragma Singleton
import QtQuick 2.0
import "."



QtObject {

    property color dangerous: '#FF5722'
    // темная тема
    property color background_d: "#212121"
    property color primary_d: "#424242"
    property color accent_d: "#428572"
    property color foreground_d: "#eeeeee"

    // светлая тема
    property color background_l: "#f5f5f5"
    property color primary_l: "#d4d4d4"
    property color accent_l: "#428572"
    property color foreground_l: "#212121"

    // тень общая для двух схем
    property color shadow: "black"

    property color background : background_d
    property color primary: primary_d
    property color accent: accent_d
    property color foreground: foreground_d

    // установка темной темы, тру - темная
    property bool settheme: false
    onSetthemeChanged: {
        if(settheme){
            background = background_d
            primary = primary_d
            accent = accent_d
            foreground = foreground_d
        }
        else{
            background = background_l
            primary = primary_l
            accent = accent_l
            foreground = foreground_l
        }
    }

    property color error: "#e53935"

    // продолжение анимации смены цвета
    property int animation_duration: 200

    // радиус углов всех блоков
    property int unite_cover_radius: 6

    // параметры тени
    property int shadow_x_offset: 0
    property int shadow_y_offset: 0
    property int shadow_blur: 4

    property int fontSizeBig:15
    property int fontSizeSmall:13

    // шрифты системы
    property FontLoader boldFont: FontLoader {
            source: "../../resources/fonts/RobotoCondensed-Bold.ttf"
        }
    property FontLoader regularFont: FontLoader {
            source: "../../resources/fonts/RobotoCondensed-Regular.ttf"
        }
    property FontLoader lightFont: FontLoader {
            source: "../../resources/fonts/RobotoCondensed-Light.ttf"
        }
}
