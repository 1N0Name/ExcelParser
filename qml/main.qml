import QtCore
import QtQuick
import QtQuick.Controls
import QtQuick.Window
import QtQuick.Layouts
import QtQuick.Dialogs
import Qt.labs.qmlmodels

import Controls 0.1

Window {
    width: 800
    height: 600
    visible: true
    title: qsTr("Excel Parser")

    ColumnLayout {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.margins: 10

        Text {
            text: qsTr("1. Выберите нужный Excel файл")
            font.family: "Segoe UI"
            color: "#7a7a7a"
            font.bold: true
            font.pixelSize: 20
        }

        CustomPathHolder {
            id: excelFilePathHolder
            isFolderDialog: false
            Layout.fillWidth: true
            Layout.topMargin: 6
            Layout.bottomMargin: 6
            Layout.rightMargin: 10

            onBrowseClicked: fileDialog.open();
        }

        FileDialog {
            id: fileDialog
            nameFilters: ["Файлы Excel (*.xlsx)"]
            fileMode: FileDialog.OpenFile
            currentFolder: StandardPaths.standardLocations(StandardPaths.DownloadLocation)[0]
            onAccepted: {
                const filename = selectedFile.toString().slice(8);
                excelFilePathHolder.text = filename;
                sheetsModel.updateFromFile(filename);
            }
        }

        Text {
            text: qsTr("2. Выберите нужный лист")
            font.family: "Segoe UI"
            color: "#7a7a7a"
            font.bold: true
            font.pixelSize: 20
        }

        RowLayout {
            spacing: 10

            CustomComboBox {
                id: sheetName
                placeholderText: qsTr("Выберите лист")
                model: sheetsModel
            }

            CustomButton {
                text: qsTr("Загрузить")
                onClicked: columnsModel.updateFromExcelSheet(fileDialog.selectedFile.toString().slice(8), sheetName.currentText)
            }
        }

        Text {
            text: qsTr("3. Выберите действие для каждой из колонок")
            font.family: "Segoe UI"
            color: "#7a7a7a"
            font.bold: true
            font.pixelSize: 20
        }

        RowLayout {
            spacing: 10

            Text {
                text: qsTr("Названия колонок: ")
                font.family: "Segoe UI"
            }

            Text {
                id: columnNames
                font.family: "Segoe UI"
            }
        }

        ColumnLayout {
            id: columnLayout
            spacing: 10

            Repeater {
                model: columnsModel
                delegate: rowLayoutDelegate
            }

            Component {
                id: rowLayoutDelegate

                RowLayout {
                    spacing: 10

                    Text {
                        text: modelData
                        font.family: "Segoe UI"
                        color: "#7a7a7a"
                        font.bold: true
                        font.pixelSize: 16
                    }

                    ComboBox {
                        model: ["Скопировать", "Проигнорировать", "Источник ключа", "источник списка"]
                    }
                }
            }
        }
    }
}
