import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts

import com.sgu.logger 1.0

Item {
    id: loggerView
    clip: true
    property alias model: logListModel

    ListModel {
        id: logListModel
    }

    ListView {
        id: listView
        anchors.fill: parent
        model: logListModel

        delegate: Text {
            text: modelData
            color: "black"
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
