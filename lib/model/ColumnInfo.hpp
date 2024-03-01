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
    QString m_name;
    ActionType m_actionType;
    int m_index;

public:
    explicit ColumnInfo(QObject* parent = nullptr) {}

    explicit ColumnInfo(const QString& name, ActionType actionType = Ignore, const int index = -1, QObject* parent = nullptr)
        : m_name(name), m_actionType(actionType), m_index(index)
    {
    }

    QString getName() const { return m_name; }
    void setName(const QString& newName) { m_name = newName; }

    ActionType getActionType() const { return m_actionType; }
    void setActionType(ActionType newActionType) { m_actionType = newActionType; }

    int getIndex() const { return m_index; }
    void setIndex(int newIndex) { m_index = newIndex; }

    static QString actionTypeToString(ActionType type)
    {
        static const char* strings[] = { "Copy", "Ignore", "KeySource", "ListSource" };
        return strings[type];
    }

private:
};

#endif // COLUMNINFO_H
