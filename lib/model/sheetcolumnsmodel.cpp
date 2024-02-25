#include "sheetcolumnsmodel.h"
#include "./../excel/excelhelper.h"

#include <QQmlContext>

SheetColumnsModel::SheetColumnsModel(QObject *parent)
    : QAbstractListModel(parent)
{}

void SheetColumnsModel::updateFromExcelSheet(const QString& docPath, const QString& sheetName) {
    beginResetModel();
    ExcelHelper eh(docPath, sheetName);
    m_columnNames = eh.getSheetColumnNames();
    endResetModel();
}

QString SheetColumnsModel::getListNames() {
    return m_columnNames.join(" ");
}

int SheetColumnsModel::rowCount(const QModelIndex &parent) const
{
    return m_columnNames.length();
}

QVariant SheetColumnsModel::data(const QModelIndex &index, int role) const
{
    if(!checkIndex(index))
        return QVariant();

    if(role == TextRole)
        return m_columnNames[index.row()];

    return QVariant();
}

QHash<int, QByteArray> SheetColumnsModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[TextRole] = "text";
    return roles;
}
