#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "lib/logger.h"
#include "lib/appcore.h"
#include "lib/model/bookheetsmodel.h"
#include "lib/model/sheetcolumnsmodel.h"

// Все, что делает эта демонстрационная программа - это выводит на экран содержимое первой ячейки Excel-файла

// В main создается объект "главного" класса Appcore и запускается приложение.
// В конструкторе Appcore загружается главное окно (файл Main.qml).
// В Main.qml описан интерфейс главного окна и происходит вызов метода в Appcore для получения
// значения первой ячейки файла doc.xlsx.

// Для работы программы необходимо поместить файл doc.xlsx (есть в репозитории) в директорию билда
// (build-ExcelMinimalApp-...)

using namespace Qt::Literals::StringLiterals;

// Точка входа в программу, как и обычно
int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
    QGuiApplication app(argc, argv); // Объект Qt-приложения с графическим интерфейсом

    // Logger
    Logger::setFile(u"ExcelParser.log"_s);
    Logger::clearLogFile();
    Logger::disableFilePrint();
    Logger::disableFunctionPrint();
    qInstallMessageHandler(Logger::write);

    QQmlApplicationEngine engine;

    SheetColumnsModel columnsModel;
    engine.rootContext()->setContextProperty("columnsModel", &columnsModel);
    BookSheetsModel sheetsModel;
    engine.rootContext()->setContextProperty("sheetsModel", &sheetsModel);
    Appcore appcore;
    engine.rootContext()->setContextProperty("appcore", &appcore);

    engine.addImportPath(":/");

    app.setApplicationName("ExcelParser");

    const QUrl url(u"qml/main.qml"_s);
    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreated,
        &app, [url](QObject* obj, const QUrl& objUrl)
        {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
