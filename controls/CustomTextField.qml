import QtQuick
import QtQuick.Controls.Basic

import Texts 0.1
import Themes 0.1

TextField {
    id: root

    leftPadding: 16
    rightPadding: 16

    font.pixelSize: 15
    font.weight: 400
    font.letterSpacing: -0.05
    verticalAlignment: TextInput.AlignVCenter

    background: Rectangle {
        id: rootBG
        implicitWidth: 150
        implicitHeight: 48
        color: ColorThemes.tonal
        radius: 12
        border.width: root.activeFocus? 1 : 0
        border.color: ColorThemes.main_color
    }
}
