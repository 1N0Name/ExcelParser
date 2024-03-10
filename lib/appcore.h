#ifndef APPCORE_H
#define APPCORE_H

#include <QMutex>
#include <QObject>

class AppCore : public QObject
{
    Q_OBJECT

public:
    static AppCore& instance()
    {
        static QMutex mutex;
        QMutexLocker locker(&mutex);
        static AppCore instance;
        return instance;
    }

    Q_INVOKABLE void openFile(const QString& path);

private:
    explicit AppCore(QObject* parent = nullptr);
    AppCore(const AppCore&)            = delete;
    AppCore& operator=(const AppCore&) = delete;
};

#endif // !APPCORE_H
