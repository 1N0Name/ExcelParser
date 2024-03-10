#include <QDebug>
#include <QFileInfo>
#include <QString>
#include <QVector>

#include "ExcelHelper.h"

ExcelHelper::ExcelHelper()
    : m_doc(std::make_unique<QXlsx::Document>()) {}

ExcelHelper::ExcelHelper(const QString& documentPath)
    : ExcelHelper()
{
    createOrLoadDocument(documentPath);
}

ExcelHelper::ExcelHelper(const QString& documentPath, const QString& sheetName)
    : ExcelHelper(documentPath)
{
    setWorkSheet(sheetName);
}

ExcelHelper::~ExcelHelper() {};

bool ExcelHelper::createOrLoadDocument(const QString& documentPath, const QString& sheetName)
{
    if (documentPath.isEmpty())
        return false;

    m_documentPath = documentPath;
    m_doc          = std::make_unique<QXlsx::Document>(documentPath);

    if (!m_doc->load() && !sheetName.isEmpty()) {
        m_doc->addSheet(sheetName);
    }
    // qInfo() << "Открываю документ: " << m_documentPath;

    return saveDocument();
}

bool ExcelHelper::setWorkSheet(const QString& sheetName)
{
    if (!m_doc->selectSheet(sheetName)) {
        qWarning() << "Лист" << sheetName << "не существует. Лист будет создан.";
        if (createNewSheet(sheetName))
            return saveDocument();
        return false;
    }
    return true;
}

QStringList ExcelHelper::getBookSheetNames() const
{
    return m_doc ? m_doc->sheetNames() : QStringList();
}

bool ExcelHelper::createNewSheet(const QString& sheetName)
{
    if (m_doc->addSheet(sheetName)) {
        m_doc->selectSheet(sheetName);
        return saveDocument();
    }
    qWarning() << "Ошибка при создании нового листа:" << sheetName;
    return false;
}

bool ExcelHelper::setColumnHeader(const int index, const ColumnInfo::ActionType& type, const QString& name) const
{
    if (!m_doc->selectSheet(m_doc->currentSheet()->sheetName())) {
        qWarning() << "Ошибка при выборе текущего листа.";
        return false;
    }

    QXlsx::Format headerFormat;
    headerFormat.setFontBold(true);
    headerFormat.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
    switch (type) {
        case ColumnInfo::Copy:
            headerFormat.setPatternBackgroundColor(QColor("#ccc0da"));
            break;
        case ColumnInfo::KeySource:
            headerFormat.setPatternBackgroundColor(QColor("#c0504d"));
            break;
        case ColumnInfo::ListSource:
            headerFormat.setPatternBackgroundColor(QColor("#fabf8f"));
            break;
        default:
            break;
    }

    m_doc->write(1, index, name, headerFormat);
    return saveDocument();
}

bool ExcelHelper::autoSizeCoumns()
{
    return m_doc->autosizeColumnWidth();
}

QStringList ExcelHelper::getSheetColumnNames() const
{
    QStringList columnNames;
    for (int col = 1; col <= getColumnCount(); ++col)
        columnNames.append(readCell(1, col));
    return columnNames;
}

[[nodiscard]] QString ExcelHelper::readCell(int row, int col) const
{
    return m_doc->read(row, col).toString();
}

bool ExcelHelper::writeCell(int row, int col, const QVariant& value)
{
    if (m_doc && row > 0 && col > 0) {
        return m_doc->write(row, col, value);
    }
    return false;
}

[[nodiscard]] int ExcelHelper::getRowCount() const
{
    return m_doc->dimension().rowCount();
}

[[nodiscard]] int ExcelHelper::getColumnCount() const
{
    return m_doc->dimension().columnCount();
}

[[nodiscard]] XlCell ExcelHelper::getCell(int row, int col) const
{
    return XlCell(row, col, m_doc->read(row, col));
}

const QString& ExcelHelper::getDocumentPath() const
{
    return m_documentPath;
}

bool ExcelHelper::docExists() const
{
    return !m_documentPath.isEmpty() && QFileInfo::exists(m_documentPath);
}

bool ExcelHelper::saveDocument() const
{
    if (m_documentPath.isEmpty()) {
        qWarning() << "Путь к документу пустой. Сохранение невозможно.";
        return false;
    }
    return m_doc->saveAs(m_documentPath);
}
