import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt5Compat.GraphicalEffects

import Themes 0.1

ComboBox {
    id: root
    textRole: "text"

    property color activeColor: ColorThemes.main_color_hover
    property color inActiveColor: ColorThemes.main_color
    property string placeholderText: ""

    currentIndex: placeholderText === "" ? 0 : -1
    displayText: currentIndex === -1 ? placeholderText : currentText

    property bool isActive: false

    delegate: ItemDelegate {
        width: root.width - 10

        property variant modelData: model

        contentItem: Text {
            text: modelData.text
            color: ColorThemes.white
            elide: Text.ElideRight
            verticalAlignment: Text.AlignVCenter
        }

        background: Rectangle {
            width: parent.width
            height: parent.height
            color: root.highlightedIndex === index ? root.activeColor : root.inActiveColor
            radius: 5

            Rectangle {
                color: "#00008B"
                visible: root.highlightedIndex === index ? true : false

                width: 3
                height: 20
                radius: 5

                anchors.left: parent.left
                anchors.leftMargin: 0
                anchors.verticalCenter: parent.verticalCenter
            }
        }
    }

    indicator: Image {
        id: indicatorIcon
        x: root.width - width - 10
        y: (root.availableHeight - height) / 2

        source: 'qrc:/down_arrow.svg'
        sourceSize: Qt.size(12, 12)

        ColorOverlay {
            anchors.fill: indicatorIcon
            source: indicatorIcon

            color: root.down ? "#DE61619c" : "#e5c1d5"

            antialiasing: true
        }

        Behavior on rotation {
            NumberAnimation {
                duration: 75
            }
        }
    }

    contentItem: Item {
        anchors.fill: parent

        Text {
            anchors.fill: parent
            leftPadding: 10
            rightPadding: indicatorIcon.width + leftPadding +10
            verticalAlignment: Text.AlignVCenter
            text: root.displayText
            elide: Text.ElideRight
            color: ColorThemes.white
        }
    }

    background: Rectangle {
        id: rootBG
        implicitWidth: 140
        implicitHeight: 48
        color: root.down ? root.activeColor : root.inActiveColor
        radius: 12

        layer.enabled: root.hovered | root.down
    }

    popup: Popup {
        y: root.height - 1
        width: root.width
        implicitHeight: contentItem.implicitHeight + 10
        padding: 5

        contentItem: ListView {
            implicitHeight: contentHeight
            model: root.popup.visible ? root.delegateModel : null
            clip: true
            currentIndex: root.highlightedIndex

            ScrollIndicator.vertical: ScrollIndicator { }
        }

        background: Rectangle {
            color: root.inActiveColor
            radius: 12
            clip: true
        }

        onVisibleChanged: visible ? indicatorIcon.rotation = 180 : indicatorIcon.rotation = 0
    }
}
