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
    property string imgSource
    property color color: enabled ? ColorThemes.main_color : "#E1E1E1"
    property bool isFolderDialog: true

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
            anchors.right: parent.right
            anchors.top: parent.top
            width: 80
            height: parent.height

            Rectangle
            {
                radius: 12
                anchors.fill: parent
                color: mouseArea.containsMouse ? ColorThemes.main_color_hover : ColorThemes.main_color

                Rectangle
                {
                    anchors.left: parent.left
                    height: parent.height
                    width: 10
                    color: mouseArea.containsMouse ? ColorThemes.main_color_hover : ColorThemes.main_color
                }

                Text
                {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    color: root.enabled ? "white" : "#929292"
                    font.pixelSize: 16
                    font.weight: Font.Medium
                    text: qsTr("Обзор")
                }
            }

            MouseArea
            {
                id: mouseArea
                hoverEnabled: true
                anchors.fill: parent
                onClicked: root.browseClicked()
                cursorShape: Qt.PointingHandCursor
            }
        }

        Flickable
        {
            id: flickable
            anchors.left: borderRect.left
            anchors.right: chooseButton.left
            anchors.bottom: parent.bottom
            height: parent.height
            width: parent.width
            clip: true
            interactive: true
            flickableDirection: Flickable.HorizontalFlick
            contentWidth: textArea.width
            contentHeight: textArea.height

            TextArea
            {
                background: Item
                {
                    implicitHeight: parent.height
                    implicitWidth: parent.height
                    visible: false
                }

                selectByMouse: true
                selectByKeyboard: true
                height: 40
                id: textArea
                readOnly: true
                font.pixelSize: 16
                text: root.text
                color: "#7a7a7a"
                topPadding: 10
            }

            ScrollBar.horizontal: ScrollBar
            {
                policy: ScrollBar.AlwaysOn
                size: flickable.width
                height: 10
                visible: flickable.contentWidth > flickable.width
                position: flickable.position.x / (flickable.contentWidth - flickable.width)
            }
        }
    }
}
