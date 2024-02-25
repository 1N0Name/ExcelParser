#ifndef BOOKSHEETSMODEL_H
#define BOOKSHEETSMODEL_H

#include <QAbstractListModel>

class BookSheetsModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit BookSheetsModel(QObject *parent = nullptr);

    enum SheetRoles {
        TextRole = Qt::UserRole + 1
    };

    /* ------------------------ Q_INVOKABLES ------------------------ */
    Q_INVOKABLE void updateFromFile(QString docPath);
    Q_INVOKABLE QString getListNames();

    QHash<int, QByteArray> roleNames() const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
    QStringList m_sheetNames;
};

#endif // !BOOKSHEETSMODEL_H
