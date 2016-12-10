#include "log.h"

QFile logFile(LOG_FILE_NAME);
bool writeLogFile(QtMsgType type, const QString msg)
{
    if(!logFile.open( QIODevice::Text | QIODevice::Append))
        return false;
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
    logFile.write(writeData.toStdString().c_str());
    logFile.close();
    return true;
}
