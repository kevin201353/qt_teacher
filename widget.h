#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QDialog>
#include <QEvent>
#include <QThread>
#include <MyGlobalShortCut/MyGlobalShortCut.h>

#include "callname.h"
#include "sendfile.h"
#include "setserverip.h"
#include "stulist.h"
#include "notice.h"
#include "thread.h"
#include <QList>


namespace Ui {
class Widget;
}

class GetDataFromTeacher;
class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
signals:
    void ShowNotice(QString szMsg);

private slots:
    void on_demo_clicked();
    void on_raiseHandPushButton_clicked();
    void exit_widget();
    void on_stu_clicked();
    void NoticeMsg(QString szMsg);
public:
    QString getHostMacAddress();
    void procesdata();
    void getclassinfo();
    void setvmclass(QString str1, QString str2);
    void SetNoticeMsg2(QString szMsg);
    void readstulistfromserver();
    Thread* GetThread(){
        return m_pthread;
    };
    DataThread* GetDataThread(){
        return m_pDataThread;
    };
private:
    Ui::Widget  *ui;
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
    QString  m_strMac;
    stulist     *stulistWidget;
    Thread   *m_pthread;
    DataThread *m_pDataThread;
    bool    m_bstart;
    classThread *m_pclassThrd;
};
#endif // WIDGET_H
