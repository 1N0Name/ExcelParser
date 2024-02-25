#ifndef APPCORE_H
#define APPCORE_H

#include <QObject>
#include <QQmlApplicationEngine>
#include <QQmlContext>

class Appcore : public QObject // Без наследования от QObject макросы Q_INVOKABLE и прочее будут недоступны
{
    Q_OBJECT // Необходимый макрос для классов-наследников от QObject
public:
    explicit Appcore(QObject *parent = nullptr); // Конструктор класса
    Q_INVOKABLE QString readFirstCell(QString docPath); // Метод для чтения первой ячейки Excel-документа.
                                                        // Q_INVOKABLE необходим для того, чтобы этот метод
                                                        // можно было вызывать из QML.
    Q_INVOKABLE QStringList getColumnNames(const QString& docPath, const QString& sheetName);

private:

};

#endif // !APPCORE_H
