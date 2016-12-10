#include <QDebug>
#include "streamparsexml.h"
#include <QFile>
#include <QMessageBox>
#include "global.h"
#include "thread.h"

//<?xml version="1.0" encoding="UTF-8">
//<COMMAND>
//    <OBJECT>USER</OBJECT>
//    <ACTION>LOGIN</ACTION>
//    <DATA>
//        <USER NAME="root" PASSWORD="123456" />
//    </DATA>
//</COMMAND>

QList<StruInfo> g_stu2List;

StreamParseXml::StreamParseXml()
{
    m_stop = false;
}

StreamParseXml::~StreamParseXml()
{

}

int nfirst = 1;
int StreamParseXml::readXml(const QString xmlData)
{
    reader = new QXmlStreamReader(xmlData);
    while(!reader->atEnd() && !reader->hasError())
    {
        if (m_stop)
           break;
        QXmlStreamReader::TokenType token = reader->readNext();
        if(token == QXmlStreamReader::StartDocument)
        {
            continue;
        }
        //内部测试数据
//        if (m_ntype == 3)
//        {
//            StruInfo info2;
//            info2.apId="ddddd";
//            info2.name="VM-10";
//            info2.noSeat="class-1";
//            noticeMsgWindow(info2);
//        }
        //end;

        if (m_ntype == 0 || m_ntype == 1 || m_ntype == 2)
        {
            //test data
            //if (nfirst == 1)
//            {
//                QString str="zhao";
//                QString strTmp="A5-8";
//                QString apId="1235678";
//                StruInfo info;
//                info.name = str;
//                info.noSeat = strTmp;
//                info.apId = apId;
//                g_stu2List.append(info);
//                noticeMsgWindow(info);
//                nfirst = 2;
//            }
            //test end
            if (reader->isStartElement() && reader->name() == "desktopRoomAps")
            {
                QString apId = getValue("apId");
                QString strTmp = getValue("dSeat");
                QString strSin = getValue("dSignin");
                if (strSin == "1")
                {
                    QString str = getValue("dStuName");
                    if (str.isEmpty() && strTmp.isEmpty())
                        continue;
                    bool insert = 0;
                    if (g_stu2List.size() > 0)
                    {
                        for (int i=0; i< g_stu2List.size(); i++)
                        {
                            StruInfo info = g_stu2List.at(i);
                            if (info.name == str && info.noSeat != strTmp)
                            {
                                insert = 1;
                                break;
                            }
                            if (info.name != str)
                            {
                                insert = 1;
                                break;
                            }
                        }
                        if (insert == 1)
                        {
                            StruInfo info;
                            info.name = str;
                            info.noSeat = strTmp;
                            info.apId = apId;
                            g_stu2List.append(info);
                            insert = 0;
                            noticeMsgWindow(info);
                        }
                    }//if g_stru > 0
                    if (g_stu2List.size() == 0)
                    {
                        StruInfo info;
                        info.name = str;
                        info.noSeat = strTmp;
                        info.apId = apId;
                        g_stu2List.append(info);
                        noticeMsgWindow(info);
                    }
                }//str == 1
            }//if
        }//m_ntype
        if (m_ntype == 3)
        {
            QString desktopName = getValue("desktopName");
            QString roomName = getValue("roomName");
            QString className = getValue("className");
            StruInfo info;
            info.apId = desktopName;
            info.name = roomName;
            info.noSeat = className;
        }
    }//
    if (reader->hasError())
        qDebug() << reader->errorString();
    reader->clear();
    delete reader;
    reader = NULL;
    return 0;
}

int StreamParseXml::readXml2(const QString filename)
{
    if(filename.isEmpty())
        return -1;
    QFile *file = new QFile(filename);
    if(!file->open(QFile::ReadOnly | QFile::Text))
    {
       QMessageBox::information(NULL, QString("title"), QString("open error!"));
       return -1;
    }
    reader = new QXmlStreamReader(file);
    while(!reader->atEnd() && !reader->hasError())
    {
        QXmlStreamReader::TokenType token = reader->readNext();
        if(token == QXmlStreamReader::StartDocument)
        {
            continue;
        }
        if (reader->isStartElement() && reader->name() == "OBJECT")
        {
             QString elementText = reader->readElementText();
             if (elementText == "USER")
             {
                 parseUserInformation();
                 break;
             }
        }
    }
    if (reader->hasError()) {
       qDebug() << reader->errorString();
       //QMessageBox::information(NULL, QString("parseXML"), reader->errorString());
    }
    reader->clear();
    delete reader;
    reader = NULL;
    return 0;
}

void StreamParseXml::parseUserInformation()
{
    QString elementString = getValue("ACTION");
    if (elementString == "LOGIN")
    {
        while(!reader->atEnd())
        {
            reader->readNext();
            if (reader->name() == "USER")
            {
                QXmlStreamAttributes attributes = reader->attributes();
                if(attributes.hasAttribute("NAME"))
                    qDebug() << "USER=" << attributes.value("NAME").toString();
                if(attributes.hasAttribute("PASSWORD"))
                    qDebug() << "PASSWORD=" << attributes.value("PASSWORD").toString();
            }
        }
    }
}

QString StreamParseXml::getValue(const QString &name)
{
    while(!reader->atEnd())
    {
        if (m_stop)
            break;
        reader->readNext();
        if (reader->isStartElement() && reader->name() == name)
            return reader->readElementText();
    }
    return "";
}

void StreamParseXml::noticeMsgWindow(StruInfo info)
{
    ReportMsg msg;
    if (m_ntype == 0 || m_ntype == 1)
    {
        msg.action = USER_MSG_DEALSTULIST;
        msg.val1 = m_ntype;
        if (m_ntype == 0)
        {
            QString strTmp = info.name;
            strTmp += " - ";
            strTmp += info.noSeat;
            strTmp += "举手";
            DataThread *pthread = (DataThread *)g_mapObject["DataThread"];
            emit pthread->NoticeShow(strTmp);
        }
    }
    if (m_ntype == 3)
    {
        msg.action = USER_MSG_CLASSINFO;
        msg.str = info.apId;  //桌面名称
        msg.strval = info.noSeat; //班级
    }
    call_msg_back(msg_respose, msg);
}
