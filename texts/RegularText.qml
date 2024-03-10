import QtQuick
import QtQuick.Controls.Basic

import Themes 0.1

Text {
    id: root

    font.family: "Inter";
    color: ColorThemes.black

    property int textSize: 15

    font.pixelSize: textSize
    font.weight: 400
    lineHeight: getLineHeight(textSize)
    lineHeightMode: Text.FixedHeight
    font.letterSpacing: getLetterSpacing(textSize)

    function getLineHeight(size) {
        switch (size) {
            case 35: return 44
            case 29: return 38
            case 24: return 32
            case 22: return 28
            case 20: return 26
            case 17: return 22
            case 16: return 20
            case 15: return 20
            case 14: return 20
            default: return 1.2 * size
        }
    }

    function getLetterSpacing(size) {
        switch (size) {
            case 35: return -0.2
            case 29: return -0.15
            case 24: return -0.15
            case 22: return -0.15
            case 20: return -0.1
            case 17: return -0.1
            case 16: return -0.1
            case 15: return -0.05
            case 14: return 0
            default: return 0
        }
    }
}
