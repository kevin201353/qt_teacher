#include "myhttp.h"

myHttp::myHttp()
{
    m_strIP = "http://";
    m_pNetManager = new QNetworkAccessManager();
    m_peventLoop = new QEventLoop();
    m_preply = NULL;
    QObject::connect(m_pNetManager, SIGNAL(finished(QNetworkReply*)), m_peventLoop, SLOT(quit()));
}

myHttp::~myHttp()
{
    if(m_pNetManager)
        delete m_pNetManager;
    if(m_peventLoop)
        delete m_peventLoop;
}
bool myHttp::Post(QString url, const QString append)
{
    QUrl    Tempurl(m_strIP+url);
    QByteArray  appen(append.toStdString().c_str());
    m_preply = m_pNetManager->post(QNetworkRequest(Tempurl),appen);
    if(NULL == m_preply)
        return false;
    return true;
}
bool myHttp::Get(QString url)
{
    QUrl    Tempurl(m_strIP+url);
    m_preply = m_pNetManager->get(QNetworkRequest(Tempurl));
    if(NULL == m_preply)
        return false;
    return true;
}
void myHttp::GetData(QString &Buf)
{
    if(Buf == NULL)
        return ;
    if(m_preply == NULL)
        return ;
    m_peventLoop->exec();
    m_XmlMessage = m_preply->readAll();
    Buf = m_XmlMessage;
}
