#ifndef APPCORE_H
#define APPCORE_H

#include <QMutex>
#include <QObject>

#include "model/ColumnInfo.hpp"

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

private:
    explicit AppCore(QObject* parent = nullptr);
    AppCore(const AppCore&)            = delete;
    AppCore& operator=(const AppCore&) = delete;
};

#endif // !APPCORE_H
