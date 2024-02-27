#include "AppCore.h"
#include "excel/ExcelHelper.h"

AppCore::AppCore(QObject* parent)
    : QObject(parent)
{
}

QString AppCore::readFirstCell(QString docPath)
{
    ExcelHelper eh(docPath);
    if (eh.docExists())
        return eh.readCell(1, 1);
    return "";
}

QStringList AppCore::getColumnNames(const QString& docPath, const QString& sheetName)
{
    ExcelHelper eh(docPath, sheetName);
    return eh.getSheetColumnNames();
}
