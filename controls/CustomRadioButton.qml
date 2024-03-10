import QtQuick
import QtQuick.Controls.Basic
import Qt5Compat.GraphicalEffects

import Themes 0.1
import Texts 0.1

RadioButton {
    id: root

    property int hPadding: 16
    property int vPadding: 10

    text: modelData
    spacing: 24
    leftPadding: hPadding
    rightPadding: hPadding
    LayoutMirroring.enabled: true

    indicator: Rectangle {
        implicitWidth: 20
        implicitHeight: 20

        x: root.mirrored ? root.width - implicitWidth - root.hPadding : root.hPadding
        y: root.topPadding + (root.availableHeight - height) / 2

        radius: width / 2
        border.width: 1
        border.color: root.checked ? ColorThemes.main_color : ColorThemes.black

        Rectangle {
            x: (parent.width - width) / 2
            y: (parent.height - height) / 2
            width: 20
            height: 20
            radius: width / 2
            color: root.checked? ColorThemes.main_color : ColorThemes.background
            visible: root.checked

            Image
            {
                id: selectedIcon
                source: 'qrc:/check.svg'
                sourceSize: Qt.size(12, 12)
                opacity: 0
                visible: parent.visible
                anchors.centerIn: parent

                Behavior on opacity {
                    NumberAnimation {
                        from: 0
                        to: 1
                        duration: 300
                        easing.type: Easing.InOutQuad
                    }
                }

                ColorOverlay {
                    anchors.fill: selectedIcon
                    source: selectedIcon
                    color: ColorThemes.white
                    antialiasing: true
                }

                onVisibleChanged: visible? selectedIcon.opacity = 0 : selectedIcon.opacity = 1
            }
        }
    }

    contentItem: MediumText {
        text: parent.text
        textSize: 16
        color: root.checked ? ColorThemes.main_color : ColorThemes.black
        leftPadding: !root.mirrored ? root.indicator.width + root.spacing : 0
        rightPadding: root.mirrored ? root.indicator.width + root.spacing : 0
        verticalAlignment: Text.AlignVCenter
    }

    background: Rectangle {
        implicitHeight: 48
        radius: 12
        color: ColorThemes.background
        border.color: root.checked ? ColorThemes.main_color : ColorThemes.border_20
        border.width: 1

        MouseArea {
            anchors.fill: parent
            enabled: false
            cursorShape: Qt.PointingHandCursor
        }
    }
}
