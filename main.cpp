#include <QDateTime>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "lib/AppCore.h"
#include "lib/excel/ExcelParser.h"
#include "lib/Logger.hpp"
#include "lib/model/BookSheetsModel.h"
#include "lib/model/SheetColumnsModel.h"

using namespace Qt::Literals::StringLiterals;

int main(int argc, char* argv[])
{
    QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
    QGuiApplication app(argc, argv);

    qInstallMessageHandler([](QtMsgType type, const QMessageLogContext& context, const QString& msg)
        { Logger::instance().write(type, context, msg); });
    qInfo().noquote() << "=============================== Новый запуск ===============================";
    qInfo().noquote() << "Время запуска: " + QDateTime::currentDateTime().toString("dd-MM-yyyy HH:mm:ss");
    qInfo().noquote() << "Имя: " << QCoreApplication::applicationName();
    qInfo().noquote() << "Версия приложения: " << QCoreApplication::applicationVersion();
    qInfo().noquote() << "ОС: " << QSysInfo::prettyProductName();
    qInfo().noquote() << "Архитектура ОС: " << QSysInfo::currentCpuArchitecture();
    qInfo().noquote() << "Версия ОС: " << QSysInfo::kernelVersion();
    qInfo().noquote() << "============================================================================";

    QQmlApplicationEngine engine;
    qmlRegisterSingletonType<Logger>("com.sgu.logger", 1, 0, "Logger", [](QQmlEngine* engine, QJSEngine* scriptEngine) -> QObject*
        { Q_UNUSED(engine) Q_UNUSED(scriptEngine) return &Logger::instance(); });

    engine.rootContext()->setContextProperty("appcore", &AppCore::instance());
    SheetColumnsModel columnsModel;
    engine.rootContext()->setContextProperty("columnsModel", &columnsModel);
    ExcelParser excelParser(&columnsModel);
    engine.rootContext()->setContextProperty("excelParser", &excelParser);
    BookSheetsModel sheetsModel;
    engine.rootContext()->setContextProperty("sheetsModel", &sheetsModel);

    engine.addImportPath(":/");

    app.setApplicationName("ExcelParser");
    app.setApplicationVersion("0.1.0");

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
