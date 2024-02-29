#ifndef SHEETCOLUMNSMODEL_H
#define SHEETCOLUMNSMODEL_H

#include <QAbstractListModel>
#include <QMap>
#include <QStringList>

#include "ColumnInfo.hpp"

class SheetColumnsModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit SheetColumnsModel(QObject* parent = nullptr);

    enum ColumnRoles
    {
        TextRole = Qt::UserRole + 1,
        ActionRole
    };

    /* ------------------------ Q_INVOKABLES ------------------------ */
    Q_INVOKABLE void updateFromExcelSheet(const QString& docPath, const QString& sheetName);
    Q_INVOKABLE bool setColumnAction(int index, ColumnInfo::ActionType action);
    Q_INVOKABLE bool verifyColumns() const;
    Q_INVOKABLE void setGroupingColumnName(const QString& name);
    Q_INVOKABLE void setKeyColumnName(const QString& name);

    const QVector<ColumnInfo>& getColumns() const;
    QString getGroupingColumnName() const;
    QString getKeyColumnName() const;

    QHash<int, QByteArray> roleNames() const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

private:
    bool isListSourcePresent() const;
    bool isKeySourceAlreadySet() const;

    QVector<ColumnInfo> m_columns;
    QString m_keyColumnName      = "KWS_UFID";
    QString m_groupingColumnName = "GROUP";
};

#endif // !SHEETCOLUMNSMODEL_H
