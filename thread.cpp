#include "thread.h"
#include <QDebug>
#include "log.h"
#include "myhttp.h"
#include "global.h"
#include "thread.h"
#include <QMutex>
#include <QMutexLocker>

QString g_xmldata;
QMutex g_mutex;
extern QMap<QString, QObject *> g_mapObject;
Thread::Thread()
{
    m_stopped = false;
}

Thread::~Thread()
{
}

void Thread::run()
{
    while (!m_stopped)
    {
        QMutexLocker Locker(&g_mutex);
        SendMessage();
        sleep(5);
    }
    m_stopped = false;
}

void Thread::stop()
{
    m_stopped = true;
}

void Thread::setMessage(QList<QString> msglist)
{
    m_msgList = msglist;
}

void Thread::SendMessage()
{
    qDebug()<<"start send message.";
    if (m_msgList.size() > 0)
    {
        qDebug()<<"start send request.";
        QString szUrl = m_msgList.at(0);
        QString szMac = m_msgList.at(1);
        szUrl += "?";
        szUrl += szMac;
        myHttp http;
        if (!http.Get(szUrl))
        //if (!http.Post(szUrl, szMac))
        {
            writeLogFile(QtDebugMsg, "sendMessage http request failed.");
        }
        http.GetData(g_xmldata);
        qDebug() << g_xmldata;
    }
}

/***********************************************************************************************/
//data process
DataThread::DataThread()
{
    m_stopped = false;
}

DataThread::~DataThread()
{

}

void DataThread::stop()
{
     m_stopped = true;
     parsexml.setStop(m_stopped);
}

void DataThread::run()
{
    while (!m_stopped)
    {
        processdata();
        sleep(3);
    }
    m_stopped = false;
}

void DataThread::processdata()
{
    QMutexLocker Locker(&g_mutex);
    if (g_xmldata.length() > 0 && !g_xmldata.isEmpty())
    {
        parsexml.setType(m_ntype);
        parsexml.readXml(g_xmldata);
        g_xmldata = "";
    }
}
