#ifndef NOTICE_H
#define NOTICE_H

#include <QWidget>
#include <QPoint>
#include <QTimer>
#include <QDebug>
#include "myqlist.h"

namespace Ui {
class Notice;
}

class Notice : public QWidget
{
    Q_OBJECT

public:
    explicit Notice(QWidget *parent = 0);
    ~Notice();
public:
    inline void printTest(){
        qDebug() << "printTest";
    };
    void setMsg(StruInfo* info);
private:
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
private slots:
    void myMove();
    void myStay();
    void myClose();
    void ShowText(StruInfo* info);
private:
    Ui::Notice *ui;
    QTimer *timerShow;
    QTimer *timerStay;
    QTimer *timerClose;
    bool isEnter;
    QPoint normal_Point;
    QString m_szMsg;
    StruInfo m_stuinfo;
};

#endif // NOTICE_H
