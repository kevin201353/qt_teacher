#ifndef LOG_H
#define LOG_H

#include <QFile>
#include <QDateTime>
#include <QString>

#define LOG_FILE_NAME "VmStuLog.txt"
#define GET_TIME (" " + QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss") + " ")
#define DEBUG    ("debug: " + GET_TIME + __FILE__ + "(" + __LINE__ + ")\n\t")
#define INFO     ("info: " + GET_TIME + __FILE__ + "(" + __LINE__ + ")\n\t")
#define WARNING  ("warning: " + GET_TIME + __FILE__ + "(" + __LINE__ + ")\n\t")
#define CRITICAL ("critical: " + GET_TIME + __FILE__ + "(" + __LINE__ + ")\n\t")
#define FATAL    ("fatal: " + GET_TIME + __FILE__ + "(" + __LINE__ + ")\n\t")

bool writeLogFile(QtMsgType type, const QString msg);
#endif // LOG_H
