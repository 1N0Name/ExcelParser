import QtQuick
import QtQuick.Controls.Basic

import Themes 0.1

Button {
    id: root

    height: 48
    padding: 0
    horizontalPadding: 24

    property color textColor        : ColorThemes.white
    property int textAlignment      : Qt.AlignCenter
    property var buttonDynamicColor : root.down ? ColorThemes.main_color_hover : root.hovered
                                                ? ColorThemes.main_color_hover : ColorThemes.main_color

    contentItem: Text {
        id: contentText
        text: root.text
        color: textColor
        font.weight: Font.Medium
        font.pixelSize: 14
        horizontalAlignment: textAlignment
        verticalAlignment: Qt.AlignVCenter
        elide: Qt.ElideRight
    }

    TextMetrics {
        id: contentTextMetrics
        font: contentText.font
        text: contentText.text
    }

    background: Rectangle {
        id: rootBG
        implicitHeight: root.height
        implicitWidth: contentTextMetrics.width + root.horizontalPadding * 2
        color: root.buttonDynamicColor
        radius: 12
    }

    MouseArea {
        anchors.fill: parent
        enabled: false
        cursorShape: Qt.PointingHandCursor
    }
}
