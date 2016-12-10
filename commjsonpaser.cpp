#include "commjsonpaser.h"
#include <QFile>
#include <QDebug>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonDocument>

commjsonpaser::commjsonpaser()
{

}

QByteArray commjsonpaser::readJsonFile(QString filename)
{
    QByteArray byte_array;
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "open json file error";
    }
    byte_array = file.readAll();
    file.close();
    return byte_array;
}

QJsonArray commjsonpaser::getArrayLastValueByKey(QStringList keyList, QByteArray byteArray)
{
    QJsonArray jsonArray;
    QJsonParseError  json_error;
    QJsonDocument parse_doucment = QJsonDocument::fromJson(byteArray, &json_error);
    if (json_error.error == QJsonParseError::NoError)
    {
        if (!(parse_doucment.isEmpty() || parse_doucment.isNull()))
        {
            QJsonObject jsonObject = parse_doucment.object();
            jsonArray = jsonObject.value(keyList.at(0)).toArray();
            return jsonArray;
        }
    }
    return jsonArray;
}

QJsonValue commjsonpaser::getObjectValueByKey(QStringList key, QByteArray byteArray)
{
    QJsonValue jsonValue;
    QJsonParseError  json_error;
    QJsonDocument parse_doucment = QJsonDocument::fromJson(byteArray, &json_error);
    if (json_error.error == QJsonParseError::NoError)
    {
        if (!(parse_doucment.isEmpty() || parse_doucment.isNull()))
        {
            if (parse_doucment.isObject())
            {
                jsonValue = parse_doucment.object().value(key.at(0));
                return jsonValue;
            }
        }
    }
    return jsonValue.Null;
}
