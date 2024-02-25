#include <QString>
#include <QVector>

#include "excelhelper.h"
#include "./../logger.h"

ExcelHelper::ExcelHelper(const QString &documentPath, const QString &sheetName)
    : m_documentPath(documentPath), m_doc(new QXlsx::Document(documentPath))
{
    setWorkSheet(sheetName);
}

ExcelHelper::~ExcelHelper()
{
    delete m_doc;
}

[[nodiscard]] bool ExcelHelper::docExists() const
{
    return m_doc && !m_documentPath.isEmpty() && m_doc->load();
}

[[nodiscard]] const QString& ExcelHelper::getDocumentPath() const
{
    return m_documentPath;
}

void ExcelHelper::setDocument(const QString &documentPath) {
    m_documentPath = documentPath;
    delete m_doc;
    m_doc = new QXlsx::Document(documentPath);
}

void ExcelHelper::setWorkSheet(const QString& sheetName)
{
    if (sheetName.isEmpty())
        return;
    m_doc->selectSheet(sheetName);
}

[[nodiscard]] QStringList ExcelHelper::getBookSheetNames() const
{
    return docExists() ? m_doc->sheetNames() : QStringList();
}

[[nodiscard]] QStringList ExcelHelper::getSheetColumnNames() const
{
    QStringList columnNames;
    for (auto i = 1; i < this->getColumnCount(); i++)
        columnNames.append(this->readCell(1, i));
    LOGD("Column Names: " << columnNames.join(" "));
    return columnNames;
}

[[nodiscard]] QString ExcelHelper::readCell(int row, int col) const
{
    return docExists() ? m_doc->read(row, col).toString() : QString();
}

[[nodiscard]] int ExcelHelper::getRowCount() const
{
    return docExists() ? m_doc->dimension().rowCount() : 0;
}

[[nodiscard]] int ExcelHelper::getColumnCount() const{

    return docExists() ? m_doc->dimension().columnCount() : 0;
}

[[nodiscard]] XlCell ExcelHelper::getCell(int row, int col) const
{
    return docExists() ? XlCell(row, col, m_doc->read(row, col)) : XlCell();
}
