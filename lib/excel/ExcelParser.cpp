#include <QFileInfo>
#include <QtConcurrent>
#include <stdexcept>

#include "ExcelHelper.h"
#include "ExcelParser.h"

struct ParseError : public std::runtime_error
{
    using std::runtime_error::runtime_error;
};

ExcelParser::ExcelParser(SheetColumnsModel* model)
    : m_columnsModel(model) {}

void ExcelParser::parseAsync(const QString& filePath, const QString& folderPath)
{
    QFuture<void> future = QtConcurrent::run([this, filePath, folderPath]()
        { this->processExcelFile(filePath, folderPath); });
}

bool ExcelParser::processExcelFile(const QString& filePath, const QString& folderPath)
{
    ExcelHelper source(filePath);

    // Определяем полный путь для целевого документа
    QString fullPath = folderPath + "/result_" + QDateTime::currentDateTime().toString("yyyy-MM-dd_HH-mm-ss") + ".xlsx";
    ExcelHelper target(fullPath, QFileInfo(filePath).fileName());

    // Получаем колонки и производим фильтрацию, исключая игнорируемые
    auto columns = m_columnsModel->getColumns();
    QVector<ColumnInfo> filteredColumns;
    std::copy_if(columns.begin(), columns.end(), std::back_inserter(filteredColumns),
        [](const ColumnInfo& column)
        { return column.getActionType() != ColumnInfo::Ignore; });

    // Установка заголовков для отфильтрованных колонок в целевом документе
    int columnIndex = 1;
    for (const auto& column : filteredColumns)
        if (column.getActionType() == ColumnInfo::Copy)
            target.setColumnHeader(columnIndex++, column.getActionType(), column.getName());

    // Добавляем колонки группировки и ключа в конец
    target.setColumnHeader(columnIndex++, ColumnInfo::ListSource, m_columnsModel->getGroupingColumnName());
    target.setColumnHeader(columnIndex, ColumnInfo::KeySource, m_columnsModel->getKeyColumnName());

    int rowCount  = source.getRowCount();
    int targetRow = 2; // Поскольку первая строка используется для заголовков

    // Проход по всем строкам исходного документа
    for (int row = 2; row <= rowCount; ++row) {
        QString keySourceValue;
        QVector<QString> copyValues;
        copyValues.reserve(filteredColumns.size());

        // Сбор значений из Copy и KeySource колонок
        std::for_each(filteredColumns.cbegin(), filteredColumns.cend(), [&](const auto& column)
            {
            QString value = source.readCell(row, column.getIndex());
            if (column.getActionType() == ColumnInfo::Copy) {
                copyValues.push_back(value);
            } else if (column.getActionType() == ColumnInfo::KeySource) {
                keySourceValue = value;
            } });

        // Обработка колонок ListSource
        std::for_each(filteredColumns.cbegin(), filteredColumns.cend(), [&](const auto& column)
            {
            if (column.getActionType() == ColumnInfo::ListSource) {
                try {
                    QString listSourceValue = source.readCell(row, column.getIndex());
                    auto numbers = parseListSource(listSourceValue);
                    QSet<QString> identifiers;

                    if (!keySourceValue.isEmpty()) {
                        if (!parseKeySource(keySourceValue, numbers, identifiers))
                            return;

                        for (const auto number : numbers) {
                            int colIndex = 1;
                            for (const auto& copyValue : copyValues) {
                                target.writeCell(targetRow, colIndex++, copyValue);
                            }
                            target.writeCell(targetRow, colIndex++, column.getName());
                            QString identifier = keySourceValue + QString::number(number).rightJustified(6, '0');
                            target.writeCell(targetRow, colIndex, identifier);
                            ++targetRow;
                        }
                    } else {
                        throw ParseError("Ключ пустой");
                    }
                } catch (const ParseError& e) {
                    qWarning() << "Невалидное знач. в строке " << row << " колонка " << column.getName() << ". " << e.what();
                }
            } });
    }
    target.autoSizeCoumns();
    qInfo() << "Обработка файла завершена!";
    emit fileParsed(fullPath);

    return target.saveDocument();
}

void ExcelParser::setColumnsModel(SheetColumnsModel* model)
{
    m_columnsModel = model;
}

QVector<int> ExcelParser::parseListSource(const QString& input)
{
    QVector<int> numbers;
    QString cleanedInput = input.simplified().replace(" ", "");

    static QRegularExpression regex("(\\d+)(?:-(\\d+))?");

    auto matchIterator = regex.globalMatch(cleanedInput);

    while (matchIterator.hasNext()) {
        auto match     = matchIterator.next();
        int start      = match.captured(1).toInt();
        QString endStr = match.captured(2);

        if (!endStr.isEmpty()) {
            int end = endStr.toInt();
            if (start > end || start > 9999 || end > 9999)
                throw ParseError(QString("Недопустимый диапазон или число превышает 9999: от %1 до %2").arg(start).arg(end).toStdString());
            for (int i = start; i <= end; ++i)
                numbers.push_back(i);
        } else {
            if (start > 9999)
                throw ParseError(QString("Число превышает 9999: %1").arg(start).toStdString());
            numbers.push_back(start);
        }
    }

    return numbers;
}

bool ExcelParser::parseKeySource(const QString& key, const QVector<int>& listSource, QSet<QString>& identifiers)
{
    if (key.length() != 11 || key.toLongLong() == 0)
        throw ParseError(QString("Неверная длина ключа или некорректное значение: %1").arg(key).toStdString());

    for (int number : listSource) {
        QString identifier = key + QString("%1").arg(number, 6, 10, QChar('0'));
        if (identifiers.contains(identifier))
            throw ParseError(QString("Обнаружен дубликат идентификатора: %1").arg(identifier).toStdString());
        identifiers.insert(identifier);
    }

    return true;
}
