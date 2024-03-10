import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts

import com.sgu.logger 1.0
import Texts 0.1
import Themes 0.1

Rectangle {
    id: loggerView
    clip: true
    radius: 12
    color: ColorThemes.tonal
    border.width: 1
    border.color: ColorThemes.main_color

    property alias model: logListModel

    ListModel {
        id: logListModel
    }

    ListView {
        id: listView
        anchors.fill: parent
        anchors.margins: 5
        model: logListModel

        delegate: TerminalText {
            text: modelData
            wrapMode: Text.WordWrap
            width: listView.width
        }

        ScrollBar.vertical: ScrollBar { }
    }

    Component.onCompleted: {
        Logger.messageLogged.connect(function(message) {
            logListModel.append({"log": message});
            listView.positionViewAtIndex(logListModel.count - 1, ListView.Contain);
        });
    }
}
