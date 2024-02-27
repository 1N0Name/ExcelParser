#ifndef SHEETCOLUMNSMODEL_H
#define SHEETCOLUMNSMODEL_H

#include <QAbstractListModel>
#include <QMap>
#include <QSet>
#include <QStringList>

class SheetColumnsModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit SheetColumnsModel(QObject* parent = nullptr);

    enum ColumnRoles
    {
        TextRole = Qt::UserRole + 1,
        ActionRole,
        CustomNameRole
    };

    enum ActionType
    {
        Copy = 0,
        Ignore,
        KeySource,
        ListSource
    };

    /* ------------------------ Q_INVOKABLES ------------------------ */
    Q_INVOKABLE void updateFromExcelSheet(const QString& docPath, const QString& sheetName);
    Q_INVOKABLE QString getListNames();
    Q_INVOKABLE bool setColumnAction(int index, ActionType action, const QString& customName = QString());
    Q_INVOKABLE bool checkColumnNamesUniqueness() const;

    QHash<int, QByteArray> roleNames() const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

private:
    struct ColumnInfo
    {
        QString name;
        ActionType actionType = Ignore;
        QString customName;
    };

    QVector<ColumnInfo> m_columns;

    bool isKeySourceAlreadySet() const;
};

#endif // !SHEETCOLUMNSMODEL_H
