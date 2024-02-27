import QtQuick
import QtQuick.Controls.Basic

ScrollBar {
    id: root

    property int contentItemOffset: 2
    property var scrollTarget: parent

    width: 8
    policy: ScrollBar.AlwaysOn

    topPadding: 4
    bottomPadding: 4

    contentItem: Rectangle {
        id: vbarIndicator
        color: "#d3d3d3"
        radius: 111

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: root.hovered ? root.contentItemOffset : !root.pressed
                                           ? root.contentItemOffset * 2 : root.contentItemOffset
        anchors.rightMargin: root.hovered ? root.contentItemOffset : !root.pressed
                                            ? root.contentItemOffset * 2 : root.contentItemOffset
    }

    background: Rectangle {
        id: vbarBG
        border.width: 1
        border.color: "red"
        // visible: root.hovered || root.pressed
        color: "#e8e8e8"
        radius: 111

        opacity: root.hovered ? 1 : !root.pressed ? 0 : 1

        Behavior on opacity {
            NumberAnimation { duration: 100 }
        }
    }
}
