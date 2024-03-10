#ifndef EXCELPARSER_H
#define EXCELPARSER_H

#include <QDebug>
#include <QObject>
#include <QRegularExpression>
#include <QSet>
#include <QVector>

#include "../model/SheetColumnsModel.h"

class ExcelParser : public QObject
{
    Q_OBJECT

public:
    explicit ExcelParser(SheetColumnsModel* model);

    Q_INVOKABLE void parseAsync(const QString& filePath, const QString& folderPath);
    void setColumnsModel(SheetColumnsModel* model);

signals:
    void fileParsed(const QString& filePath);

private:
    bool processExcelFile(const QString& filePath, const QString& folderPath);
    static QVector<int> parseListSource(const QString& input);
    static bool parseKeySource(const QString& key, const QVector<int>& listSource, QSet<QString>& identifiers);

    SheetColumnsModel* m_columnsModel = nullptr; // Не владеем объектом, только ссылка
};

#endif // EXCELPARSER_H
