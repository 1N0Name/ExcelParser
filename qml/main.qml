import QtCore
import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Window
import QtQuick.Layouts
import QtQuick.Dialogs
import Qt.labs.qmlmodels

import Texts 0.1
import Controls 0.1
import Themes 0.1

Window {
    width: 1000
    height: 800
    visible: true
    title: qsTr("Excel Parser")
    color: ColorThemes.background

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

        function getWindowsPath(path) {
            return path.toString().slice(8);
        }
    }

    ColumnLayout {
        spacing: 10
        anchors.fill: parent
        anchors.margins: 10

        ScrollView {
            id: scrollView
            Layout.fillWidth: true
            Layout.fillHeight: true
            contentWidth: mainContent.width
            contentHeight: mainContent.height

            ColumnLayout {
                id: mainContent
                spacing: 24

                ColumnLayout {
                    spacing: 20

                    MediumText {
                        text: qsTr("1. Выберите нужный Excel файл")
                        textSize: 24
                        wrapMode: Text.WordWrap
                    }

                    CustomPathHolder {
                        id: excelFilePathHolder
                        Layout.fillWidth: true
                        Layout.preferredHeight: 48
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
                }

                ColumnLayout {
                    spacing: 20

                    MediumText {
                        text: qsTr("2. Выберите нужный лист")
                        textSize: 24
                        wrapMode: Text.WordWrap
                    }

                    RowLayout {
                        spacing: 10

                        ButtonGroup {
                            id: themeRG
                            exclusive: true
                        }

                        Repeater {
                            model: sheetsModel

                            CustomRadioButton {
                                ButtonGroup.group: themeRG
                                onClicked: columnsModel.updateFromExcelSheet(internal.getWindowsPath(fileDialog.selectedFile), text)
                            }
                        }
                    }
                }

                ColumnLayout
                {
                    spacing: 20

                    MediumText {
                        text: qsTr("3. Выберите действие для колонок")
                        textSize: 24
                        wrapMode: Text.WordWrap
                    }

                    ColumnLayout {
                        id: sheetColumnsLayout
                        spacing: 16

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
                                spacing: 24

                                property int currentIndex: index

                                MediumText {
                                    text: model.text
                                    textSize: 20
                                    Layout.preferredWidth: 200
                                }

                                CustomComboBox {
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
                                }
                            }
                        }
                    }
                }

                ColumnLayout
                {
                    spacing: 20

                    MediumText {
                        text: qsTr("4. Выберите названия для колонок")
                        textSize: 24
                        wrapMode: Text.WordWrap
                    }

                    RowLayout
                    {
                        spacing: 24

                        ColumnLayout
                        {
                            spacing: 8

                            MediumText {
                                text: qsTr("Группировка:")
                                textSize: 16
                            }

                            CustomTextField {
                                id: groupFieldName
                                placeholderText: "GROUP"
                                Layout.preferredWidth:  200
                                onTextChanged: columnsModel.setGroupingColumnName(text)
                            }
                        }

                        ColumnLayout
                        {
                            spacing: 8

                            MediumText {
                                text: qsTr("Результирующая:")
                                textSize: 16
                            }

                            CustomTextField {
                                id: keyFieldName
                                placeholderText: "KWS_UFID"
                                Layout.preferredWidth:  200
                                onTextChanged: columnsModel.setKeyColumnName(text)
                            }
                        }
                    }
                }

                ColumnLayout
                {
                    spacing: 20

                    MediumText {
                        text: qsTr("5. Выберите результирующую директорию")
                        textSize: 24
                        wrapMode: Text.WordWrap
                    }

                    CustomPathHolder {
                        id: excelFolderPathHolder
                        Layout.fillWidth: true
                        Layout.preferredHeight: 48
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
                }

                RowLayout {
                    spacing: 24

                    CustomButton {
                        text: qsTr("Начать обработку")
                        onClicked: {
                            if(!columnsModel.verifyColumns()) {
                                errorDialog.title = "Ошибка. Проверьте ошибки в логе.";
                                errorDialog.open();
                            } else {
                                excelParser.parseAsync(internal.getWindowsPath(fileDialog.selectedFile),
                                                       internal.getWindowsPath(folderDialog.selectedFolder));
                            }
                        }
                    }

                    RegularText {
                        id: openFileText
                        enabled: false
                        text: enabled? "Открыть файл" : "Доступно после завершения обработки"
                        color: enabled? ColorThemes.main_color : ColorThemes.tonal
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

        LoggerView {
            id: logger
            Layout.fillWidth: true
            Layout.preferredHeight: 200
        }
    }
}
