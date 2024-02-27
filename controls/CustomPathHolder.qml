import QtQuick
import QtQuick.Controls.Basic
import Qt5Compat.GraphicalEffects

import Themes 0.1

Item {
    id: root

    width: 20
    height: 48

    signal browseClicked

    property string text: "Путь не задан"
    property color color: enabled ? ColorThemes.main_color : "#E1E1E1"

    Rectangle
    {
        id: borderRect
        width: root.width
        border.width: 1
        border.color: root.enabled ? ColorThemes.main_color : "#E1E1E1"
        radius: 12
        antialiasing: true
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        clip: true

        Item
        {
            id: chooseButton
            width: 80
            height: parent.height
            anchors.right: parent.right
            anchors.top: parent.top

            RoundRectangle
            {
                radius: 12
                radiusCorners: Qt.AlignRight | Qt.AlignTop | Qt.AlignBottom
                anchors.fill: parent
                color: mouseArea.containsMouse ? ColorThemes.main_color_hover : ColorThemes.main_color

                Text
                {
                    text: qsTr("Обзор")
                    font.weight: Font.Medium
                    font.pixelSize: 14
                    color: root.enabled ? "white" : "#929292"
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                }
            }

            MouseArea
            {
                id: mouseArea
                anchors.fill: parent
                hoverEnabled: true
                cursorShape: Qt.PointingHandCursor
                onClicked: root.browseClicked()
            }
        }

        Flickable
        {
            id: flickable
            anchors.left: borderRect.left
            anchors.right: chooseButton.left
            anchors.bottom: parent.bottom
            width: parent.width
            height: parent.height
            contentWidth: textArea.width
            contentHeight: textArea.height
            clip: true
            interactive: true
            flickableDirection: Flickable.HorizontalFlick

            TextArea
            {
                id: textArea
                readOnly: true
                height: 40
                topPadding: 10
                selectByMouse: true
                selectByKeyboard: true

                font.pixelSize: 16
                text: root.text
                color: "#7a7a7a"

                background: Item
                {
                    implicitHeight: parent.height
                    implicitWidth: parent.height
                    visible: false
                }
            }

            ScrollBar.horizontal: ScrollBar
            {
                size: flickable.width
                height: 10
                visible: flickable.contentWidth > flickable.width
                policy: ScrollBar.AlwaysOn
            }
        }
    }
}
