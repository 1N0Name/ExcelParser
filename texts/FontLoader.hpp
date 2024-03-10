#ifndef FONTLOADER_H
#define FONTLOADER_H

#include <QDebug>
#include <QDir>
#include <QFontDatabase>

class FontLoader
{
public:
    static void loadFontsFromDirectory(const QString& directoryPath)
    {
        QDir dir(directoryPath);
        if (!dir.exists()) {
            qWarning() << "Directory does not exist:" << directoryPath;
            return;
        }

        QStringList fontFilters = { "*.ttf", "*.ttc", "*.otf", "*.woff", "*.woff2" };
        dir.setNameFilters(fontFilters);

        for (auto& fontFile : dir.entryList(QDir::Files)) {
            auto filePath = dir.absoluteFilePath(fontFile);
            int fontId    = QFontDatabase::addApplicationFont(filePath);
            if (fontId == -1) {
                qWarning() << "Failed to load font:" << filePath;
            } else {
                for (const auto& family : QFontDatabase::applicationFontFamilies(fontId)) {
                    qDebug() << "Loaded font family:" << family;
                }
            }
        }
    }
};

#endif // FONTLOADER_H
