#include "myqlist2.h"

MyQList2::MyQList2(QObject *parent) : QObject(parent)
{
}

void MyQList2::append(StruInfo info)
{
    QMutexLocker Locker(&m_mutex);
    m_List.append(info);
}

StruInfo& MyQList2::at(int i)
{
    QMutexLocker Locker(&m_mutex);
    return (*(StruInfo*)&m_List.at(i));
}

void MyQList2::removeAt(int i)
{
    QMutexLocker Locker(&m_mutex);
    m_List.removeAt(i);
}

void MyQList2::clear()
{
    QMutexLocker Locker(&m_mutex);
    m_List.clear();
}

int MyQList2::size()
{
    QMutexLocker Locker(&m_mutex);
    return m_List.size();
}
