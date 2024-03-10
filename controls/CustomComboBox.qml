import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts
import Qt5Compat.GraphicalEffects

import Themes 0.1
import Texts 0.1

ComboBox {
    id: root

    property string placeholderText: ""
    currentIndex: placeholderText === "" ? 0 : -1
    displayText: currentIndex === -1 ? placeholderText : currentText

    delegate: ItemDelegate {
        required property var model
        required property int index

        width: root.width

        contentItem: RowLayout {
            spacing: 16

            RegularText {
                text: model[root.textRole]
                elide: Text.ElideRight
                verticalAlignment: Text.AlignVCenter
                Layout.fillWidth: true
            }

            Image {
                source: 'qrc:/check.svg'
                sourceSize: Qt.size(16, 16)
                visible: root.currentIndex === index ? true : false
            }
        }

        background: RoundRectangle {
            width: parent.width
            height: parent.height
            color: root.highlightedIndex === index ? ColorThemes.light_blue : ColorThemes.background
            border.width: 1
            border.color: ColorThemes.border_20

            MouseArea {
                anchors.fill: parent
                enabled: false
                cursorShape: Qt.PointingHandCursor
            }
        }
    }

    indicator: Image {
        id: indicatorIcon
        x: root.width - width - 10
        y: (root.availableHeight - height) / 2

        source: 'qrc:/down_arrow.svg'
        sourceSize: Qt.size(20, 20)

        ColorOverlay {
            anchors.fill: indicatorIcon
            source: indicatorIcon
            color: ColorThemes.black
            antialiasing: true
        }

        Behavior on rotation {
            NumberAnimation {
                duration: 75
            }
        }
    }

    contentItem: RegularText {
        property int horizontalPadding: 16

        anchors.fill: parent
        text: root.displayText
        textSize: 15
        elide: Text.ElideRight
        verticalAlignment: Text.AlignVCenter
        leftPadding: horizontalPadding
        rightPadding: indicatorIcon.width + 2 * horizontalPadding
    }

    background: RoundRectangle {
        id: rootBG
        color: ColorThemes.tonal
        implicitHeight: 48
        radius: 12
        radiusCorners: !root.down ? Qt.AlignLeft | Qt.AlignRight | Qt.AlignTop | Qt.AlignBottom :
                                    Qt.AlignLeft | Qt.AlignRight | Qt.AlignTop
        layer.enabled: root.hovered | root.down

        MouseArea {
            anchors.fill: parent
            enabled: false
            cursorShape: Qt.PointingHandCursor
        }
    }

    popup: Popup {
        y: root.implicitHeight
        width: root.implicitWidth
        implicitHeight: contentItem.implicitHeight
        padding: 0

        contentItem: ListView {
            implicitHeight: contentHeight
            model: root.popup.visible ? root.delegateModel : null
            currentIndex: root.highlightedIndex

            ScrollIndicator.vertical: ScrollIndicator { }
        }

        background: RoundRectangle {
            color: ColorThemes.background
            clip: true
            radius: 12
            radiusCorners: Qt.AlignLeft | Qt.AlignRight | Qt.AlignBottom
        }

        onVisibleChanged: visible ? indicatorIcon.rotation = 180 : indicatorIcon.rotation = 0
    }
}
