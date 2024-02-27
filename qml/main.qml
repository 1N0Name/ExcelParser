import QtCore
import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Window
import QtQuick.Layouts
import QtQuick.Dialogs
import Qt.labs.qmlmodels

import Controls 0.1

Window {
    width: 1000
    height: 800
    visible: true
    title: qsTr("Excel Parser")

    QtObject {
        id: internal

        property var actionTypes: {
            "copy": 0,
            "ignore": 1,
            "keySource": 2,
            "listSource": 3
        }

        function actionText(index) {
            switch (index) {
                case actionTypes.copy: return "Скопировать";
                case actionTypes.ignore: return "Проигнорировать";
                case actionTypes.keySource: return "Источник ключа";
                case actionTypes.listSource: return "Источник списка";
                default: return "";
            }
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10

        ScrollView {
            id: scrollView
            Layout.fillHeight: true
            Layout.fillWidth: true

            ScrollBar.vertical: CustomScrollBar { id: vbar; }

            ColumnLayout {
                width: Math.max(implicitWidth, scrollView.availableWidth)
                spacing: 10

                Text {
                    text: qsTr("1. Выберите нужный Excel файл")
                    font.family: "Segoe UI"
                    color: "#7a7a7a"
                    font.bold: true
                    font.pixelSize: 20
                }

                CustomPathHolder {
                    id: excelFilePathHolder
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

                GridLayout {
                    id: sheetColumnsLayout
                    columns: 2

                    Dialog {
                        id: errorDialog
                        modal: true
                        title: "Ошибка"

                        standardButtons: Dialog.Ok
                    }

                    Repeater {
                        id: columnsActions
                        model: columnsModel
                        delegate: RowLayout {
                            id: columnDelegate
                            spacing: 10

                            property int currentIndex: index

                            Text {
                                text: model.text
                                font.family: "Segoe UI"
                                color: "#7a7a7a"
                                font.bold: true
                                font.pixelSize: 16
                            }

                            ComboBox {
                                id: actionComboBox
                                property int previousIndex: 0
                                model: Object.keys(internal.actionTypes).map(key => internal.actionText(internal.actionTypes[key]))

                                onActivated: {
                                    var newName = columnNameField.visible && columnNameField.text !== "" ? columnNameField.text : columnNameField.placeholderText;
                                    if (!columnsModel.setColumnAction(columnDelegate.currentIndex, currentIndex, newName)) {
                                        if (currentIndex === internal.actionTypes.keySource) {
                                            currentIndex = previousIndex;
                                            errorDialog.title = qsTr("Источник ключа уже выбран. Выберите другой тип действия для этого столбца.");
                                            errorDialog.open();
                                        }
                                    } else {
                                        previousIndex = currentIndex;
                                    }
                                }
                                Layout.preferredWidth: 150
                            }

                            TextField {
                                id: columnNameField
                                visible: [internal.actionTypes.keySource, internal.actionTypes.listSource].includes(actionComboBox.currentIndex)
                                placeholderText: actionComboBox.currentIndex === internal.actionTypes.keySource ? "KWS_UFID" : "GROUP"
                                onTextChanged: {
                                    var name = text !== "" ? text : placeholderText;
                                    if (columnsModel.setColumnAction(columnDelegate.currentIndex, actionComboBox.currentIndex, name)) {
                                        errorDialog.title = "Названия столбцов должны быть уникальны.";
                                        errorDialog.open();
                                    }
                                }
                                Layout.preferredWidth: 150
                            }
                        }
                    }
                }

                Text {
                    text: qsTr("4. Выберите результирующую директорию")
                    font.family: "Segoe UI"
                    color: "#7a7a7a"
                    font.bold: true
                    font.pixelSize: 20
                }

                CustomPathHolder {
                    id: excelFolderPathHolder
                    Layout.fillWidth: true
                    Layout.topMargin: 6
                    Layout.bottomMargin: 6
                    Layout.rightMargin: 10

                    onBrowseClicked: folderDialog.open();
                }

                FolderDialog {
                    id: folderDialog
                    currentFolder: StandardPaths.standardLocations(StandardPaths.DownloadLocation)[0]
                    onAccepted: {
                        const filename = selectedFolder.toString().slice(8);
                        excelFolderPathHolder.text = filename;
                    }
                }

                RowLayout {
                    spacing: 10

                    Text {
                        text: qsTr("5. Начать парсинг")
                        font.family: "Segoe UI"
                        color: "#7a7a7a"
                        font.bold: true
                        font.pixelSize: 20
                    }

                    CustomButton {
                        text: qsTr("Обработать")
                        onClicked: {
                            console.log("Процесс запущен...")
                        }
                    }
                }
            }
        }

        Logger {
            id: logger
            Layout.fillWidth: true
            Layout.preferredHeight: 200
        }
    }
}
