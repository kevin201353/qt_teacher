#ifndef MYQLIST2_H
#define MYQLIST2_H

#include <QObject>
#include <QMutex>
#include <QMutexLocker>
#include "global.h"

class MyQList2 : public QObject
{
    Q_OBJECT
public:
    explicit MyQList2(QObject *parent = 0);
public:
    void append(StruInfo info);
    StruInfo& at(int i);
    void removeAt(int i);
    void clear();
    int size();
    inline QList<StruInfo>& GetList() {
        return m_List;
    }
private:
    QList<StruInfo> m_List;
    QMutex  m_mutex;
};

#endif // MYQLIST2_H
