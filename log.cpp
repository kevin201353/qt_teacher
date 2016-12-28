#include "log.h"
#include <QMutexLocker>
QMutex g_mutexfile;
//QFile logFile(LOG_FILE_NAME);
mylog g_mylog;
bool writeLogFile(QtMsgType type, const QString msg)
{
    QMutexLocker Locker(&g_mutexfile);
//    if(!logFile.open( QIODevice::Text | QIODevice::Append))
//        return false;
//    QString writeData;
//    writeData.clear();
//    switch(type)
//    {
//    case QtDebugMsg:
//        writeData = DEBUG;
//        break;
//    case QtInfoMsg:
//        writeData = INFO;
//        break;
//    case QtWarningMsg:
//        writeData = WARNING;
//        break;
//    case QtCriticalMsg:
//        writeData = CRITICAL;
//        break;
//    case QtFatalMsg:
//        writeData = FATAL;
//        break;
//    }

//    writeData += msg;
//    writeData += "\n";
//    logFile.write(writeData.toStdString().c_str());
//    logFile.close();

    g_mylog.writeLogFile(type, msg);
    return true;
}

/*******************************************************/
//log 文件
mylog::mylog()
{
    m_bopening = false;
    m_pLog = new QFile(LOG_FILE_NAME);
    Q_ASSERT(m_pLog != NULL);
}

mylog::~mylog()
{
    if (m_pLog)
    {
        delete m_pLog;
        m_pLog = NULL;
    }
    m_bopening = false;
}

bool mylog::open()
{
    if(!m_pLog->open( QIODevice::Text | QIODevice::Append))
        return false;
    m_bopening = true;
    return true;
}

void mylog::writeLogFile(QtMsgType type, const QString msg)
{
    if (m_bopening && m_pLog != NULL)
    {
        QString writeData;
        writeData.clear();
        switch(type)
        {
        case QtDebugMsg:
            writeData = DEBUG;
            break;
        case QtInfoMsg:
            writeData = INFO;
            break;
        case QtWarningMsg:
            writeData = WARNING;
            break;
        case QtCriticalMsg:
            writeData = CRITICAL;
            break;
        case QtFatalMsg:
            writeData = FATAL;
            break;
        }

        writeData += msg;
        writeData += "\n";
        m_pLog->write(writeData.toStdString().c_str());
    }
}

void mylog::close()
{
    if (m_pLog != NULL && m_bopening)
    {
        m_pLog->close();
    }
}
