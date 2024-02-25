#ifndef SHEETCOLUMNSMODEL_H
#define SHEETCOLUMNSMODEL_H

#include <QAbstractListModel>

class SheetColumnsModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit SheetColumnsModel(QObject *parent = nullptr);

    enum ColumnRoles {
        TextRole = Qt::UserRole + 1
    };

    /* ------------------------ Q_INVOKABLES ------------------------ */
    Q_INVOKABLE void updateFromExcelSheet(const QString& docPath, const QString& sheetName);
    Q_INVOKABLE QString getListNames();

    QHash<int, QByteArray> roleNames() const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
    QStringList m_columnNames;
};

#endif // !SHEETCOLUMNSMODEL_H
