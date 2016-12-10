#ifndef STREAMPARSEXML_H
#define STREAMPARSEXML_H
#include <QXmlStreamReader>
#include <QObject>
#include "global.h"

class StreamParseXml
{
public:
    StreamParseXml();
    ~StreamParseXml();
    int readXml(const QString xmlData);
    int readXml2(const QString filename);
    inline void setType(int ntype){
        m_ntype = ntype;
    };
    void noticeMsgWindow(StruInfo info);
    inline void setStop(bool bstop){
        m_stop = bstop;
    };
private:
    void parseUserInformation();
    QString getValue(const QString &name);
    QString getAttribute(const QString &name);
private:
    QXmlStreamReader *reader;
    int m_ntype;  //0: 举手  1：学生
    bool  m_stop;
};

#endif // STREAMPARSEXML_H
