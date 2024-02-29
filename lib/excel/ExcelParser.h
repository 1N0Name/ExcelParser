#ifndef EXCELPARSER_H
#define EXCELPARSER_H

#include <QDebug>
#include <QRegularExpression>
#include <QSet>
#include <QVector>

class ExcelParser
{
public:
    static QVector<int> parseListSource(const QString& input)
    {
        QVector<int> numbers;
        QString cleanedInput = input.simplified().replace(" ", ""); // Удаление пробелов

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

    static bool parseKeySource(const QString& key, const QVector<int>& listSource, QSet<QString>& identifiers)
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
};

#endif // EXCELPARSER_H
