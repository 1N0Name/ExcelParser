#include <QQmlContext>

#include "../excel/ExcelParser.h"
#include "./../excel/ExcelHelper.h"
#include "SheetColumnsModel.h"

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

bool SheetColumnsModel::setColumnAction(int index, ColumnInfo::ActionType action)
{
    if (index < 0 || index >= m_columns.size() || (action == ColumnInfo::KeySource && isKeySourceAlreadySet())) {
        qDebug() << "Invalid index or KeySource already set. Index:" << index << ", Action:" << action;
        return false;
    }

    auto& column = m_columns[index];
    column.setActionType(action);

    // Сообщаем о изменении данных в модели
    QModelIndex modelIndex = createIndex(index, 0);
    emit dataChanged(modelIndex, modelIndex, { ActionRole });

    qDebug() << "Действие установлено для Индекса:" << index << "Действие:" << action;
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

bool SheetColumnsModel::verifyColumns() const
{
    bool keySourceFound = std::any_of(m_columns.begin(), m_columns.end(), [](const auto& column)
        { return column.getActionType() == ColumnInfo::KeySource; });

    if (!keySourceFound) {
        qWarning() << "Источник Ключа не найден.";
        return false;
    }

    if (!isListSourcePresent()) {
        qWarning() << "Не найдено ни одного Источника Списка.";
        return false;
    }

    qInfo() << "Названия всех результирующих колонок корректны!";
    return true;
}

const QVector<ColumnInfo>& SheetColumnsModel::getColumns() const
{
    return m_columns;
}
QString SheetColumnsModel::getGroupingColumnName() const
{
    return m_groupingColumnName;
}

QString SheetColumnsModel::getKeyColumnName() const
{
    return m_keyColumnName;
}

void SheetColumnsModel::setGroupingColumnName(const QString& name)
{
    m_groupingColumnName = name;
}

void SheetColumnsModel::setKeyColumnName(const QString& name)
{
    m_keyColumnName = name;
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
        default: return QVariant();
    }
}

QHash<int, QByteArray> SheetColumnsModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[TextRole]   = "text";
    roles[ActionRole] = "action";
    return roles;
}
