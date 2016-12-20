#ifndef GLOBAL_H
#define GLOBAL_H
#include <QMessageBox>
#include <QObject>
#include <QMap>
#include <QList>
#define MESSAGEBOX(msg, parent) ({QMessageBox msgBox(parent); \
                         msgBox.setText(msg); \
                         msgBox.exec();})

#define  USER_MSG_TABLEVIEW          101
#define  USER_MSG_DEALSTULIST        102
#define  USER_MSG_NOTICEWINDOW       103
#define  USER_MSG_CLASSINFO          104

struct XSize {
    int x;
    int y;
    int width;
    int height;
};

struct ReportMsg {
    QObject *obj;
    unsigned int action; //消息类型
    unsigned int val1;
    unsigned int val2;
    QString   str;
    QString   strval;
};

struct StruInfo {
    QString id;
    QString name;
    QString noSeat;
    QString apId;
    QString handup;
};

#define   HTTP_URL_HEAD  "http://"
#define   SERVICE_ADDRESS  "192.168.110.243"

extern QMap<QString, QObject *> g_mapObject;
typedef void (*MsgCallBackFun)(struct ReportMsg msg);
void msg_respose(struct ReportMsg msg);
void call_msg_back(MsgCallBackFun fun, struct ReportMsg msg);

#endif // GLOBAL_H
