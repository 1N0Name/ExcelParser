#ifndef COLUMNINFO_H
#define COLUMNINFO_H

#include <QString>

class ColumnInfo
{

public:
    enum ActionType
    {
        Copy = 0,
        Ignore,
        KeySource,
        ListSource
    };

private:
    QString name;
    ActionType actionType;

public:
    explicit ColumnInfo(QObject* parent = nullptr) {}

    explicit ColumnInfo(const QString& name, ActionType actionType = Ignore, const QString& customName = QString(), QObject* parent = nullptr)
        : name(name), actionType(actionType)
    {
    }

    QString getName() const { return name; }
    void setName(const QString& newName) { name = newName; }

    ActionType getActionType() const { return actionType; }
    void setActionType(ActionType newActionType) { actionType = newActionType; }

    static QString actionTypeToString(ActionType type)
    {
        static const char* strings[] = { "Copy", "Ignore", "KeySource", "ListSource" };
        return strings[type];
    }

private:
};

#endif // COLUMNINFO_H
