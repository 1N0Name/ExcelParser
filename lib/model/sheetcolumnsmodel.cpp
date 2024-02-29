#include "SheetColumnsModel.h"
#include "./../excel/ExcelHelper.h"

#include <QQmlContext>

SheetColumnsModel::SheetColumnsModel(QObject* parent)
    : QAbstractListModel(parent)
{
}

void SheetColumnsModel::updateFromExcelSheet(const QString& docPath, const QString& sheetName)
{
    beginResetModel();
    ExcelHelper eh(docPath, sheetName);
    auto columnNames = eh.getSheetColumnNames();
    m_columns.clear();

    for (const auto& name : columnNames) {
        ColumnInfo info(name);
        m_columns.append(info);
    }

    endResetModel();
}

bool SheetColumnsModel::setColumnAction(int index, ColumnInfo::ActionType action, const QString& customName)
{
    if (index < 0 || index >= m_columns.size() || (action == ColumnInfo::KeySource && isKeySourceAlreadySet())) {
        qDebug() << "Invalid index or KeySource already set. Index:" << index << ", Action:" << action;
        return false;
    }

    auto& column = m_columns[index];
    column.setActionType(action);
    column.setCustomName(customName);

    // Сообщаем о изменении данных в модели
    QModelIndex modelIndex = createIndex(index, 0);
    emit dataChanged(modelIndex, modelIndex, { ActionRole, CustomNameRole });

    qDebug() << "Action set for Index:" << index << "Action:" << action << "CustomName:" << customName;
    return true;
}

bool SheetColumnsModel::isKeySourceAlreadySet() const
{
    return std::any_of(m_columns.begin(), m_columns.end(), [](const auto& column)
        { return column.getActionType() == ColumnInfo::KeySource; });
}

bool SheetColumnsModel::isListSourcePresent() const
{
    return std::any_of(m_columns.begin(), m_columns.end(), [](const auto& column)
        { return column.getActionType() == ColumnInfo::ListSource; });
}

bool SheetColumnsModel::checkColumnNamesUniqueness() const
{
    QMultiMap<QString, QString> customNameToColumnNames;
    for (const auto& column : m_columns) {
        customNameToColumnNames.insert(column.getCustomName(), column.getName());
    }

    bool hasDuplicates = false;

    auto i = customNameToColumnNames.begin();
    while (i != customNameToColumnNames.end()) {
        auto range = customNameToColumnNames.equal_range(i.key());
        QStringList columnNames;
        for (auto it = range.first; it != range.second; ++it) {
            columnNames << it.value();
        }
        if (columnNames.size() > 1) {
            hasDuplicates = true;
            qWarning() << "Дубликаты выбранного имени:" << i.key() << "в колонках:" << columnNames.join(", ");
        }
        i = range.second;
    }

    return !hasDuplicates;
}

bool SheetColumnsModel::verifyColumns() const
{
    bool keySourceFound = std::any_of(m_columns.begin(), m_columns.end(), [](const auto& column)
        { return column.getActionType() == ColumnInfo::KeySource; });

    if (!keySourceFound) {
        qWarning() << "Источник Ключа не найден.";
        return false;
    }

    if (!checkColumnNamesUniqueness()) {
        qWarning() << "Названия колонок не уникальны.";
        return false;
    }

    if (!isListSourcePresent()) {
        qWarning() << "Не найдено ни одного Источника Списка.";
        return false;
    }

    qInfo() << "Все хорошо!";
    return true;
}

void SheetColumnsModel::setColumnHeaders()
{
    ExcelHelper eh;
    eh.setColumnHeaders(m_columns);
}

QString SheetColumnsModel::getListNames()
{
    QStringList names;
    for (const auto& column : m_columns)
        names << column.getName();
    return names.join(" ");
}

int SheetColumnsModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return m_columns.size();
}

QVariant SheetColumnsModel::data(const QModelIndex& index, int role) const
{
    if (!checkIndex(index))
        return QVariant();

    const auto& column = m_columns[index.row()];
    switch (role) {
        case TextRole: return column.getName();
        case ActionRole: return column.getActionType();
        case CustomNameRole: return column.getCustomName();
        default: return QVariant();
    }
}

QHash<int, QByteArray> SheetColumnsModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[TextRole]       = "text";
    roles[ActionRole]     = "action";
    roles[CustomNameRole] = "customName";
    return roles;
}
