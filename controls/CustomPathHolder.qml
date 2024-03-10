import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Basic
import Qt5Compat.GraphicalEffects

import Themes 0.1

Item {
    id: root

    signal browseClicked

    property string text: "Путь не задан"

    width: parent.width
    height: parent.height

    TextArea
    {
        id: textArea
        readOnly: true
        height: parent.height
        anchors.left: root.left
        anchors.right: chooseBtn.left
        anchors.rightMargin: 8
        selectByMouse: true
        selectByKeyboard: true

        text: root.text
        font.family: "Inter"
        color: ColorThemes.black
        font.pixelSize: 15
        font.weight: 400
        font.letterSpacing: -0.05
        verticalAlignment: Qt.AlignVCenter

        background: Rectangle
        {
            implicitWidth: parent.width
            implicitHeight: parent.height
            radius: 12
            color: ColorThemes.tonal
        }
    }

    CustomButton
    {
        id: chooseBtn
        text: qsTr("Обзор")
        anchors.right: root.right
        onClicked: root.browseClicked()
    }
}
