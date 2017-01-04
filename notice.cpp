#include "notice.h"
#include "ui_notice.h"
#include <QRect>
#include <QDesktopWidget>
#include "global.h"

extern bool m_bNoticeRunning;
extern MyQList   g_NoticeList;
Notice::Notice(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Notice)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_TranslucentBackground, true);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
//    QDesktopWidget* desktopWidget = QApplication::desktop();
//    QRect deskRect = desktopWidget->availableGeometry();
//    QPoint bottomright = deskRect.bottomRight();
//    setGeometry(QRect(bottomright.x()-width()-10, bottomright.y()-height()-10, width(), height()));
    //new cal
    QDesktopWidget *desktop = QApplication::desktop();
    QRect desk_rect = desktop->availableGeometry();
    normal_Point.setX(desk_rect.width() - rect().width());
    normal_Point.setY(desk_rect.height() - rect().height());
    move(normal_Point.x(), normal_Point.y());
    ui->label_Notice->setStyleSheet("background-color: rgba(158, 159, 165, 255)");
    //ui->label->setStyleSheet("border-image: url(:/images/file_receiver.png);");
    g_mapObject["noticewin"] = this;
    isEnter = false;
    timerShow = NULL;
    timerStay = NULL;
    timerClose = NULL;
    connect(g_mapObject["widget"], SIGNAL(ShowNotice(StruInfo*)), this, SLOT(ShowText(StruInfo*)));
    timerShow = new QTimer(this);
    connect(timerShow, SIGNAL(timeout()), this, SLOT(myMove()));
    timerStay = new QTimer(this);
    connect(timerStay, SIGNAL(timeout()), this, SLOT(myStay()));
    timerClose = new QTimer(this);
    connect(timerClose, SIGNAL(timeout()), this, SLOT(myClose()));
    timerShow->start(2);
    m_bNoticeRunning = true;
}

Notice::~Notice()
{
   m_bNoticeRunning = false;
   delete ui;
   if (timerShow)
   {
       timerShow->stop();
       delete timerShow;
       timerShow = NULL;
   }
   if (timerStay)
   {
       timerStay->stop();
       delete timerStay;
       timerStay = NULL;
   }
   if (timerClose)
   {
       timerClose->stop();
       delete timerClose;
       timerClose = NULL;
   }
   //qDebug() << "666666666666666666666 notice exit !";
}

void Notice::myMove()
{
    static int beginY = QApplication::desktop()->height();
    beginY--;
    move(normal_Point.x(), beginY);
    if(beginY <= normal_Point.y())
    {
        timerShow->stop();
        timerStay->start(2000);
        beginY = QApplication::desktop()->height();
    }
}

//停留显示
void Notice::myStay()
{
    static int timeCount=0;
    timeCount++;
    QString szPrint;
    szPrint = szPrint.number(timeCount, 10);
    QString szTmp = "notice mystay @@@@@@@@@@@@@@@@@@@@@@@@";
    szTmp += "    ";
    szTmp += szPrint;
    qDebug() << szTmp;
    if(timeCount >= 4 && g_NoticeList.getsize() == 0)
    {
        timerStay->stop();
        timerClose->start(200);
        timeCount = 0;
    }
}

//自动关闭时实现淡出效果
void Notice::myClose()
{
    static double tran=1.0;
    if( isEnter )
    {
        tran = 1.0;
        setWindowOpacity(tran);
        return;
    }
    tran -= 0.1;
    if( tran <= 0.0 )
    {
        timerClose->stop();
        tran = 1.0;
        //g_NoticeList.removeAll();
        g_NoticeList.remove(m_stuinfo.id);
        emit close();
        delete this;
    }
    else
    {
        setWindowOpacity(tran);
    }
}

void Notice::enterEvent(QEvent *)
{
    isEnter = true;
    myClose();
}

void Notice::leaveEvent(QEvent *)

{
    isEnter = false;
}

void Notice::ShowText(StruInfo* info)
{
    if (info != NULL)
    {
        m_stuinfo = *info;
        QString strTmp = info->name;
        strTmp += " - ";
        strTmp += info->noSeat;
        strTmp += "举手";
        ui->label_Notice->setText(strTmp);
        g_NoticeList.remove(info->id);
        show();
    }
}

void Notice::setMsg(StruInfo* info)
{
    QString strTmp = info->name;
    strTmp += " - ";
    strTmp += info->noSeat;
    strTmp += "举手";
    ui->label_Notice->setText(strTmp);
    g_NoticeList.remove(info->id);
}
