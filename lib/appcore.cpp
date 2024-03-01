#include <QDesktopServices>
#include <QUrl>

#include "AppCore.h"

AppCore::AppCore(QObject* parent)
    : QObject(parent)
{
}

Q_INVOKABLE void AppCore::openFile(const QString& path)
{
    QDesktopServices::openUrl(QUrl::fromLocalFile(path));
}
