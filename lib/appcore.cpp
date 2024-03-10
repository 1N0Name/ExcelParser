#include <QDesktopServices>
#include <QDir>
#include <QUrl>

#include "../texts/FontLoader.hpp"
#include "AppCore.h"

AppCore::AppCore(QObject* parent)
    : QObject(parent)
{
    FontLoader::loadFontsFromDirectory(QString(ROOT_PATH) + "/assets/fonts/");
}

Q_INVOKABLE void AppCore::openFile(const QString& path)
{
    QDesktopServices::openUrl(QUrl::fromLocalFile(path));
}
