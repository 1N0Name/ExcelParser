pragma Singleton

import QtQuick
import QtQuick.Controls

Item {
    id: item

    property alias themes: m_themes;

    QtObject {
        id: m_themes

        readonly property var light: [ "#111111", "#FFFFFF", "#FFFFFF", Qt.rgba(0.067, 0.067, 0.067, 0.2),
            "#F4F4F4", "#006DED", "#025FCC", Qt.rgba(0.0, 0.427, 0.929, 0.1)
        ];
    }

    property var currentTheme: themes.light

    readonly property string black:                         currentTheme[0]
    readonly property string white:                         currentTheme[1]
    readonly property string background:                    currentTheme[2]
    readonly property string border_20:                     currentTheme[3]
    readonly property string tonal:                         currentTheme[4]
    readonly property string main_color:                    currentTheme[5]
    readonly property string main_color_hover:              currentTheme[6]
    readonly property string light_blue:                    currentTheme[7]
}
