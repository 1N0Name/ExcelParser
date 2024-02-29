#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <QCoreApplication>
#include <QDateTime>
#include <QDebug>
#include <QFile>
#include <QMutex>
#include <QMutexLocker>
#include <QObject>
#include <QTextStream>

class Logger : public QObject
{
    Q_OBJECT

public:
    enum Target
    {
        DISABLED = 0,
        CONSOLE  = 1 << 0,
        LOG_FILE = 1 << 1
    };
    Q_DECLARE_FLAGS(Targets, Target)

    enum LogLevel
    {
        Debug    = 1 << 0,
        Info     = 1 << 1,
        Warning  = 1 << 2,
        Critical = 1 << 3,
        Fatal    = 1 << 4
    };
    Q_DECLARE_FLAGS(LogLevels, LogLevel)

    enum LogOption
    {
        Timestamp     = 1 << 0,
        Header        = 1 << 1,
        FilePrint     = 1 << 2,
        FunctionPrint = 1 << 3
    };
    Q_DECLARE_FLAGS(LogOptions, LogOption)

    static Logger& instance()
    {
        static Logger instance;
        return instance;
    }

    void setLogTarget(Targets targets)
    {
        QMutexLocker locker(&m_mutex);
        m_logTarget = targets;
        if (m_logTarget.testFlag(LOG_FILE) && (!m_logFile || !m_logFile->isOpen())) {
            m_logFile.reset(new QFile(QCoreApplication::applicationDirPath() + "/log.txt"));
            m_logFile->open(QFile::Append | QFile::Text);
        }
    }

    void setLogLevels(LogLevels levels)
    {
        QMutexLocker locker(&m_mutex);
        m_logLevels = levels;
    }

    void setLogOptions(LogOptions options)
    {
        QMutexLocker locker(&m_mutex);
        m_options = options;
    }

    void write(QtMsgType type, const QMessageLogContext& context, const QString& msg)
    {
        QMutexLocker locker(&m_mutex);

        if (m_logTarget & DISABLED || !m_logLevels.testFlag(static_cast<LogLevel>(1 << type)))
            return;

        QString logLine;
        if (m_options & Timestamp) {
            logLine += QDateTime::currentDateTime().toString("[dd | hh:mm:ss] ");
        }

        if (m_options & Header) {
            static const QHash<QtMsgType, QString> icons = {
                {QtDebugMsg,     QString::fromUtf8("\xf0\x9f\x9f\xa3")},
                { QtInfoMsg,     QString::fromUtf8("\xf0\x9f\x9f\xa2")},
                { QtWarningMsg,  QString::fromUtf8("\xf0\x9f\x9f\xa1")},
                { QtCriticalMsg, QString::fromUtf8("\xf0\x9f\x9f\xa0")},
                { QtFatalMsg,    QString::fromUtf8("\xf0\x9f\x94\xb4")}
            };
            logLine += icons.value(type) + " ";
        }

        if (m_options & FilePrint) {
            logLine += QString(context.file).section('/', -1) + ":" + QString::number(context.line) + " ";
        }

        if (m_options & FunctionPrint) {
            logLine += QString(context.function) + " - ";
        }

        logLine += msg;

        emit messageLogged(logLine);

        if (m_logTarget & CONSOLE) {
            qDebug().noquote() << logLine;
        }

        if (m_logTarget & LOG_FILE && m_logFile && m_logFile->isOpen()) {
            QTextStream out(m_logFile.data());
            out << logLine << "\n";
        }
    }

signals:
    void messageLogged(const QString& message);

private:
    explicit Logger(QObject* parent = nullptr)
        : QObject(parent), m_logTarget(CONSOLE | LOG_FILE), m_logLevels(LogLevel::Info | LogLevel::Warning | LogLevel::Critical | LogLevel::Fatal), m_options(Timestamp | Header)
    {
        if (m_logTarget & LOG_FILE) {
            m_logFile.reset(new QFile(QCoreApplication::applicationDirPath() + "/log.txt"));
            if (!m_logFile->open(QFile::Append | QFile::Text)) {
                m_logFile.reset();
            }
        }
    }

    ~Logger()
    {
        if (m_logFile && m_logFile->isOpen()) {
            m_logFile->close();
        }
    }

    Targets m_logTarget;
    LogLevels m_logLevels;
    LogOptions m_options;
    QMutex m_mutex;
    QScopedPointer<QFile> m_logFile;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(Logger::Targets)
Q_DECLARE_OPERATORS_FOR_FLAGS(Logger::LogLevels)
Q_DECLARE_OPERATORS_FOR_FLAGS(Logger::LogOptions)

#endif // LOGGER_HPP
