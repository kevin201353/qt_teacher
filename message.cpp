#include "global.h"
#include "cmytableview.h"
#include <QDebug>
#include "stulist.h"
#include "cmytableview.h"
#include <QStandardItem>
#include "log.h"
#include "streamparsexml.h"
#include "myhttp.h"
#include "notice.h"
#include "widget.h"
#include "global.h"

extern QList<StruInfo> g_stu2List;
extern QMap<QString, QObject *> g_mapObject;

void call_msg_back(MsgCallBackFun fun, struct ReportMsg msg)
{
    fun(msg);
}

void msg_respose(struct ReportMsg msg)
{
    switch (msg.action) {
    case USER_MSG_TABLEVIEW:
        {
            CMytableview *myTableView = qobject_cast<CMytableview *>(msg.obj);
            if (myTableView != NULL)
            {
                int col = msg.val1;
                int row = msg.val2;
                if (row >=0 && g_stu2List.size() > 0)
                {
                    char sztmp[100] = {0};
                    sprintf(sztmp, "col = %d, row = %d .", col, row);
                    StruInfo info = g_stu2List.at(row);
                    QString url = HTTP_URL_HEAD;
                    url += SERVICE_ADDRESS;
                    url += "/service/classes/specify_stu_show";
                    QString data;
                    data = "apId=";
                    data += info.apId;
                    myHttp *http = new myHttp;
                    if (http)
                    {
                        QString str = "stu list item select : ";
                        str += url;
                        str += "------";
                        str += data;
                        writeLogFile(QtDebugMsg, str);
                        if (!http->Post(url, data))
                        {
                            writeLogFile(QtDebugMsg, "教室指定学生演示失败.");
                        }
                        delete http;
                        http = NULL;
                    }
                    //MESSAGEBOX(sztmp, myTableView);
                }
            }
        }
        break;
    case USER_MSG_DEALSTULIST:
        {
            stulist *list = (stulist *)g_mapObject["stulist"];
            if (list != NULL)
            {
                CMytableview *pView = (CMytableview *)list->GetList();
                if (pView != NULL)
                {
                    pView->clearSpans();
                    int size = g_stu2List.size();
                    for(int i=0; i<size ; i++)
                    {
                        StruInfo info = g_stu2List.at(i);
                        QStandardItem * item = new QStandardItem;
                        QString str = info.name;
                        str += " - ";
                        str += info.noSeat;
                        item->setText(str);
                        if (pView->GetMyItemModel() != NULL)
                        {
                            pView->setRowHeight(i, 30);
                            if ( i*30 + 30 > pView->height())
                            {
                                //出现滚动条
                                pView->setColumnWidth(0,  522 -30);
                                pView->setColumnWidth(1, 60);
                            }
                            pView->GetMyItemModel()->setItem(i, 0, item);
                        }
                    }//for
                }//if pview
            }//if list
        }
        break;
    case USER_MSG_CLASSINFO:
        {
             Widget *pwin = (Widget *)g_mapObject["widget"];
             if (pwin)
             {
                pwin->setvmclass(msg.str, msg.strval);
             }
        }
        break;
    default:
        break;
    }
}
