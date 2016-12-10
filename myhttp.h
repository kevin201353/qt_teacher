#ifndef MYHTTP_H
#define MYHTTP_H

#include <QString>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QTextCodec>
#include <QObject>
#include <QEventLoop>

class myHttp : public QObject
{
    Q_OBJECT
public:
    myHttp();
    ~myHttp();
    void SetUrlIP(QString IP)
    {
        m_strIP.clear();
        m_strIP = "http://";
        m_strIP += IP;
    }
    bool Post(QString url, const QString append);
    void GetData(QString &Buf);
    bool Get(QString url);
private:
    QNetworkAccessManager   *m_pNetManager;
    QString m_strIP;
private:
    QString m_XmlMessage;
    QEventLoop *m_peventLoop;
    QNetworkReply *m_preply;
};

#endif // MYHTTP_H
