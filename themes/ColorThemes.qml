pragma Singleton

import QtQuick
import QtQuick.Controls

Item {
    id: item

    property alias themes: m_themes;

    QtObject {
        id: m_themes

        readonly property var light: [ "#111111", "#FFFFFF", "#FF003D", "#111111", "#FFFFFF",
            "#006DED", "#025FCC", "#111111", "#FFFFFF", "#111111", "#006DED", "#F4F4F4", "#DCDCDC",
            "#111111", "#111111", "#C4CAD2"
        ];
    }

    property var currentTheme: themes.light

    readonly property string black:                         currentTheme[0]
    readonly property string white:                         currentTheme[1]
    readonly property string error:                         currentTheme[2]
    readonly property string overlay:                       currentTheme[3]
    readonly property string background:                    currentTheme[4]
    readonly property string main_color:                    currentTheme[5]
    readonly property string main_color_hover:              currentTheme[6]
    readonly property string icons_white_and_black:         currentTheme[7]
    readonly property string text_on_primary:               currentTheme[8]
    readonly property string text_white_and_black:          currentTheme[9]
    readonly property string light_blue:                    currentTheme[10]
    readonly property string tonal:                         currentTheme[11]
    readonly property string tonal_2:                       currentTheme[12]
    readonly property string border:                        currentTheme[13]
    readonly property string border_20:                     currentTheme[14]
    readonly property string dark_grey:                     currentTheme[15]
}
