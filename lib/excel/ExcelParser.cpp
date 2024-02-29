#include <QFileInfo>

#include "ExcelHelper.h"
#include "ExcelParser.h"

ExcelParser::ExcelParser(SheetColumnsModel* model)
    : m_columnsModel(model) {}

bool ExcelParser::parse(const QString& filePath, const QString& folderPath)
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

    return true;
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
            if (start > end || start > 9999 || end > 9999) {
                qWarning() << "Invalid range or number exceeds 9999 in input:" << start << "to" << end;
                return {};
            }
            for (int i = start; i <= end; ++i) {
                numbers.push_back(i);
            }
        } else {
            if (start > 9999) {
                qWarning() << "Number exceeds 9999 in input:" << start;
                return {};
            }
            numbers.push_back(start);
        }
    }

    return numbers;
}

bool ExcelParser::parseKeySource(const QString& key, const QVector<int>& listSource, QSet<QString>& identifiers)
{
    // Проверка корректности ключа
    if (key.length() != 11 || !key.toLongLong()) {
        qWarning() << "Invalid key source:" << key;
        return false;
    }

    for (int number : listSource) {
        QString identifier = key + QString("%1").arg(number, 6, 10, QChar('0'));
        if (identifier.length() != 17 || identifiers.contains(identifier)) {
            qWarning() << "Duplicate or incorrect identifier:" << identifier;
            return false;
        }
        identifiers.insert(identifier);
    }

    return true;
}
