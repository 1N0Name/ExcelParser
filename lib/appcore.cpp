#include "appcore.h"
#include "logger.h"
#include "excel/excelhelper.h"

Appcore::Appcore(QObject *parent)
    : QObject{parent}
{ }

[[nodiscard]] QString Appcore::readFirstCell(QString docPath) // Не обращай внимание на [[nodiscard]],
                                                              // можешь это не использовать
{
    ExcelHelper eh(docPath); // ExcelHelper - созданный нами вспомогательный класс для работы с Excel
    if (eh.docExists())
        return eh.readCell(1, 1); // В Excel счет ячеек и столбцов начинается с 1
    return "";
}

QStringList Appcore::getColumnNames(const QString& docPath, const QString& sheetName)
{
    ExcelHelper eh(docPath, sheetName);
    return eh.getSheetColumnNames();
}
