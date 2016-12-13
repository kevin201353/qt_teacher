#include "myhttp.h"
#include <QDebug>
#include <QByteArray>
#include "log.h"

myHttp::myHttp()
{
    m_pNetManager = new QNetworkAccessManager(this);
    m_peventLoop = new QEventLoop();
    //m_pRequst.setRawHeader("eduToken", "ABCDEF0123456789");
    //m_pRequst.setRawHeader("Accept", "application/xml");
    QObject::connect(m_pNetManager, SIGNAL(finished(QNetworkReply*)), m_peventLoop, SLOT(quit()));
    //QObject::connect(m_pNetManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
}

myHttp::~myHttp()
{
    if (m_pNetManager)
        delete m_pNetManager;
    if (m_peventLoop)
        delete m_peventLoop;
}
bool myHttp::Post(QString url, QString append)
{
    QUrl    Tempurl(url);
    QByteArray  appen(append.toLatin1());
    //m_pRequst.setUrl(Tempurl);
    QNetworkRequest head;
    head.setRawHeader(QByteArray("eduToken"), QByteArray("ABCDEF0123456789"));
    head.setRawHeader(QByteArray("Accept"), QByteArray("application/xml"));
    //head.setRawHeader(QByteArray("Content-Type"), QByteArray("application/xml;charset=UTF-8"));
    head.setUrl(Tempurl);
    QString sTmp(appen);
    qDebug() << sTmp.toStdString().c_str();
    m_preply = m_pNetManager->post(head, appen);
    if(NULL == m_preply)
        return false;
    return true;
}
bool myHttp::Get(QString url)
{
    QUrl    Tempurl(url);
   // m_pRequst.setUrl(Tempurl);
    QNetworkRequest head;
    head.setRawHeader(QByteArray("eduToken"), QByteArray("ABCDEF0123456789"));
    head.setRawHeader(QByteArray("Accept"), QByteArray("application/xml"));
    head.setUrl(Tempurl);
    m_preply = m_pNetManager->get(head);
    if(NULL == m_preply)
        return false;
    return true;
}

//同步处理 http
void myHttp::GetData(QString &Buf)
{
    if(m_preply == NULL)
        return ;
    m_peventLoop->exec();
    m_XmlMessage = m_preply->readAll();
    Buf = m_XmlMessage;
    writeLogFile(QtDebugMsg, Buf);
    qDebug() << Buf;
}

//异步处理 http
void myHttp::replyFinished(QNetworkReply *reply)
{
    qDebug()<<reply->readAll();
}
