#include "BookSheetsModel.h"
#include "./../excel/ExcelHelper.h"

#include <QQmlContext>

BookSheetsModel::BookSheetsModel(QObject* parent)
    : QAbstractListModel(parent)
{
}

void BookSheetsModel::updateFromFile(QString docPath)
{
    beginResetModel();
    ExcelHelper eh(docPath);
    m_sheetNames = eh.getBookSheetNames();
    endResetModel();
}

QString BookSheetsModel::getListNames()
{
    return m_sheetNames.join(" ");
}

int BookSheetsModel::rowCount(const QModelIndex& parent) const
{
    return m_sheetNames.length();
}

QVariant BookSheetsModel::data(const QModelIndex& index, int role) const
{
    if (!checkIndex(index))
        return QVariant();

    if (role == TextRole)
        return m_sheetNames[index.row()];

    return QVariant();
}

QHash<int, QByteArray> BookSheetsModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[TextRole] = "text";
    return roles;
}
