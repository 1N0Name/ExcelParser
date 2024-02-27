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
        ColumnInfo info;
        info.name       = name;
        info.customName = name;
        m_columns.append(info);
    }

    endResetModel();
}

bool SheetColumnsModel::setColumnAction(int index, ActionType action, const QString& customName)
{
    // Проверка валидности индекса и предотвращение дублирования ключевого источника
    if (index < 0 || index >= m_columns.size() || (action == KeySource && isKeySourceAlreadySet())) {
        qDebug() << "Invalid index or KeySource already set. Index: " << index << ", Action: " << action;
        return false;
    }

    if (!checkColumnNamesUniqueness()) {
        qDebug() << "Custom name is not unique: " << customName;
        return false;
    }

    // Обновление действия и пользовательского имени для указанной колонки
    auto& column      = m_columns[index];
    column.actionType = action;
    // Если пользовательское имя не задано, используем существующее имя колонки
    column.customName = customName.isEmpty() ? column.name : customName;

    // Сообщаем о изменении данных в модели
    QModelIndex modelIndex = createIndex(index, 0);
    emit dataChanged(modelIndex, modelIndex, { ActionRole, CustomNameRole });

    qDebug() << "Action set for Index: " << index << " Action: " << action << " CustomName: " << customName;
    return true;
}

bool SheetColumnsModel::isKeySourceAlreadySet() const
{
    for (const auto& column : m_columns) {
        if (column.actionType == KeySource)
            return true;
    }
    return false;
}

bool SheetColumnsModel::checkColumnNamesUniqueness() const
{
    QSet<QString> uniqueNames;
    for (const auto& column : m_columns) {
        if (!column.customName.isEmpty()) {
            if (uniqueNames.contains(column.customName)) {
                return false;
            }
            uniqueNames.insert(column.customName);
        }
    }
    return true;
}

QString SheetColumnsModel::getListNames()
{
    QStringList names;
    for (const auto& column : m_columns) {
        names << column.name;
    }
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
        case TextRole: return column.name;
        case ActionRole: return column.actionType;
        case CustomNameRole: return column.customName.isEmpty() ? column.name : column.customName;
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
