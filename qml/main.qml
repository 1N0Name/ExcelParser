import QtCore
import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Window
import QtQuick.Layouts
import QtQuick.Dialogs
import Qt.labs.qmlmodels

import Controls 0.1
import Themes 0.1

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
        spacing: 10
        anchors.fill: parent
        anchors.margins: 10

        ScrollView {
            id: scrollView
            Layout.fillHeight: true
            Layout.fillWidth: true

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
                    currentFolder: StandardPaths.writableLocation(StandardPaths.DownloadLocation)
                    fileMode: FileDialog.OpenFile
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
                        onActivated: columnsModel.updateFromExcelSheet(fileDialog.selectedFile.toString().slice(8), sheetName.currentText)
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
                    columns: 3
                    uniformCellWidths: true

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
                                property int previousIndex: 1
                                currentIndex: previousIndex
                                model: Object.keys(internal.actionTypes).map(key => internal.actionText(internal.actionTypes[key]))

                                onActivated: {
                                    if (!columnsModel.setColumnAction(columnDelegate.currentIndex, currentIndex)) {
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
                        }
                    }
                }

                Text {
                    text: qsTr("4. Выберите названия для результирующей колонки и колонки группировки")
                    font.family: "Segoe UI"
                    color: "#7a7a7a"
                    font.bold: true
                    font.pixelSize: 20
                }

                RowLayout {
                    spacing: 10

                    Text {
                        text: qsTr("Группировка: ")
                        font.family: "Segoe UI"
                        color: "#7a7a7a"
                        font.bold: true
                        font.pixelSize: 16
                    }

                    TextField {
                        id: groupFieldName
                        placeholderText: "GROUP"
                        onTextChanged: columnsModel.setGroupingColumnName(text)
                        Layout.preferredWidth: 100
                    }

                    Text {
                        text: qsTr("Результирующая: ")
                        font.family: "Segoe UI"
                        color: "#7a7a7a"
                        font.bold: true
                        font.pixelSize: 16
                    }

                    TextField {
                        id: keyFieldName
                        placeholderText: "KWS_UFID"
                        onTextChanged: columnsModel.setKeyColumnName(text)
                        Layout.preferredWidth: 100
                    }
                }


                Text {
                    text: qsTr("5. Выберите результирующую директорию")
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
                    currentFolder: StandardPaths.writableLocation(StandardPaths.DocumentsLocation)
                    onAccepted: {
                        const filename = selectedFolder.toString().slice(8);
                        excelFolderPathHolder.text = filename;
                    }
                }

                RowLayout {
                    spacing: 10

                    Text {
                        text: qsTr("6. Начать парсинг")
                        font.family: "Segoe UI"
                        color: "#7a7a7a"
                        font.bold: true
                        font.pixelSize: 20
                    }

                    CustomButton {
                        text: qsTr("Обработать")
                        onClicked: {
                            if(!columnsModel.verifyColumns()) {
                                errorDialog.title = "Ошибка. Проверьте ошибки в логе.";
                                errorDialog.open();
                            } else {
                                excelParser.parse(fileDialog.selectedFile.toString().slice(8),
                                                  folderDialog.selectedFolder.toString().slice(8));
                            }
                        }
                    }

                    Text {
                        id: openFileText
                        enabled: false
                        text: enabled? "Открыть файл с результатом" : "Кнопка открытия доступна после завершения обработки"
                        color: enabled? ColorThemes.main_color : "#777777"
                        font.underline: true

                        property string filePath: ""

                        MouseArea {
                            anchors.fill: parent
                            enabled: true
                            cursorShape: Qt.PointingHandCursor

                            onClicked: {
                                appcore.openFile(openFileText.filePath)
                            }

                            Component.onCompleted: {
                                excelParser.fileParsed.connect(function(path) {
                                    openFileText.filePath = path;
                                    openFileText.enabled = true;
                                });
                            }
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
