#ifndef COMMJSONPASER_H
#define COMMJSONPASER_H
#include <QByteArray>
#include <QString>
#include <QJsonArray>
#include <QStringList>
#include <QJsonValue>

class commjsonpaser
{
public:
    commjsonpaser();
public:
    QByteArray readJsonFile(QString filename);
    QJsonArray getArrayLastValueByKey(QStringList keyList, QByteArray byteArray);
    QJsonValue getObjectValueByKey(QStringList key, QByteArray byteArray);
};

#endif // COMMJSONPASER_H
