#ifndef EXCELHELPER_H
#define EXCELHELPER_H

#include "xlsxdocument.h"
#include "xlsxworkbook.h"
#include <QString>

#include "../model/ColumnInfo.hpp"
#include "XlCell.hpp"

class ExcelHelper
{
public:
    explicit ExcelHelper();
    ExcelHelper(const QString& documentPath);
    ExcelHelper(const QString& documentPath, const QString& sheetName);
    ~ExcelHelper();

    // Методы для работы с документами и листами
    bool saveDocument() const;
    bool createOrLoadDocument(const QString& documentPath, const QString& sheetName = QString());
    bool setWorkSheet(const QString& sheetName);
    QStringList getBookSheetNames() const;
    bool createNewSheet(const QString& sheetName);
    [[nodiscard]] const QString& getDocumentPath() const;
    [[nodiscard]] bool docExists() const;

    // Методы для работы с данными
    bool setColumnHeader(const int index, const ColumnInfo::ActionType& type, const QString& name) const;
    [[nodiscard]] QStringList getSheetColumnNames() const;
    [[nodiscard]] QString readCell(int row, int col) const;
    [[nodiscard]] XlCell getCell(int row, int col) const;
    [[nodiscard]] int getRowCount() const;
    [[nodiscard]] int getColumnCount() const;

private:
    std::unique_ptr<QXlsx::Document> m_doc;
    QString m_documentPath;
};

#endif // !EXCELHELPER_H
