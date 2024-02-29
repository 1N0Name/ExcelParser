#ifndef EXCELHELPER_H
#define EXCELHELPER_H

#include "xlsxdocument.h"
#include "xlsxworkbook.h"
#include <QString>

#include "../model/ColumnInfo.hpp"
#include "XlCell.hpp"

// Этот класс содержит в себе только методы для чтения, но не записи, их надо будет добавить, если нужно
class ExcelHelper
{
public:
    explicit ExcelHelper(const QString& documentPath = QString(), const QString& sheetName = QString());
    ~ExcelHelper();

    void setDocument(const QString& documentPath);
    const QString& getDocumentPath() const;
    void setWorkSheet(const QString& sheetName);
    QStringList getBookSheetNames() const; // Получить список листов
    QStringList getSheetColumnNames() const;
    void createNewSheet(const QString& sheetName, const QVector<ColumnInfo>& columns);
    void setColumnHeaders(const QVector<ColumnInfo>& columns);
    QString readColumnByName(const QString& columnName) const;
    QString readColumnByIndex(int index) const;
    int getRowCount() const;
    int getColumnCount() const;
    QString readCell(int row, int col) const; // Считать ячейку
    XlCell getCell(int row, int col) const;   // Получить ячейку (XlCell - тоже самописный вспомогательный класс)

    bool docExists() const;

private:
    QString m_documentPath;
    QXlsx::Document* m_doc;
};

#endif // !EXCELHELPER_H
