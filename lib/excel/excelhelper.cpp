#include <QString>
#include <QVector>

#include "../Logger.hpp"
#include "ExcelHelper.h"

ExcelHelper::ExcelHelper(const QString& documentPath, const QString& sheetName)
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

void ExcelHelper::setDocument(const QString& documentPath)
{
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

void ExcelHelper::createNewSheet(const QString& sheetName, const QVector<ColumnInfo>& columns)
{
    m_doc->addSheet(sheetName);
    setWorkSheet(sheetName);
    setColumnHeaders(columns);
}

void ExcelHelper::setColumnHeaders(const QVector<ColumnInfo>& columns)
{
    if (!m_doc->selectSheet(m_doc->currentSheet()->sheetName()))
        return;

    QXlsx::Format headerFormat;
    headerFormat.setFontBold(true);
    headerFormat.setHorizontalAlignment(QXlsx::Format::AlignHCenter);

    int columnIndex = 1;
    for (const auto& column : columns) {
        if (column.getActionType() != ColumnInfo::Ignore) {
            qDebug() << ColumnInfo::actionTypeToString(column.getActionType()) << " | " << column.getCustomName();
            // m_doc->write(1, columnIndex, column.getCustomName(), headerFormat);
            // columnIndex++;
        }
    }
}

QString ExcelHelper::readColumnByName(const QString& columnName) const
{
    if (!docExists())
        return QString();

    QStringList columnNames = getSheetColumnNames();
    int columnIndex         = columnNames.indexOf(columnName) + 1;
    return readColumnByIndex(columnIndex);
}

QString ExcelHelper::readColumnByIndex(int index) const
{
    QString result;
    int rowCount = getRowCount();
    for (int i = 2; i <= rowCount; ++i) {
        result += readCell(i, index) + "\n";
    }
    return result;
}

[[nodiscard]] QStringList ExcelHelper::getSheetColumnNames() const
{
    QStringList columnNames;
    for (auto i = 1; i < this->getColumnCount(); i++)
        columnNames.append(this->readCell(1, i));
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

[[nodiscard]] int ExcelHelper::getColumnCount() const
{

    return docExists() ? m_doc->dimension().columnCount() : 0;
}

[[nodiscard]] XlCell ExcelHelper::getCell(int row, int col) const
{
    return docExists() ? XlCell(row, col, m_doc->read(row, col)) : XlCell();
}
