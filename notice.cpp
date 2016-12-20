#include "notice.h"
#include "ui_notice.h"
#include <QRect>
#include <QDesktopWidget>
#include "global.h"

extern bool m_bNoticeRunning;
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
    ui->label_Notice->setStyleSheet("background-color: rgba(255, 255, 255, 255)");
    //ui->label->setStyleSheet("border-image: url(:/images/file_receiver.png);");
    g_mapObject["noticewin"] = this;
    isEnter = false;
    connect(g_mapObject["widget"], SIGNAL(ShowNotice(QString)), this, SLOT(ShowText(QString)));
    timerShow = new QTimer(this);
    connect(timerShow, SIGNAL(timeout()), this, SLOT(myMove()));
    timerStay = new QTimer(this);
    connect(timerStay, SIGNAL(timeout()), this, SLOT(myStay()));
    timerClose = new QTimer(this);
    connect(timerClose, SIGNAL(timeout()), this, SLOT(myClose()));
    timerShow->setInterval(500);
    timerStay->setInterval(500);
    timerClose->setInterval(500);
    timerShow->start(5);
    m_bNoticeRunning = true;
}

Notice::~Notice()
{
   delete ui;
   delete timerShow;
   delete timerStay;
   delete timerClose;
   m_bNoticeRunning = false;
}

void Notice::myMove()
{
    static int beginY = QApplication::desktop()->height();
    beginY--;
    move(normal_Point.x(), beginY);
    if(beginY <= normal_Point.y())
    {
        timerShow->stop();
        timerStay->start(1000);
    }
}

//停留显示
void Notice::myStay()
{
    static int timeCount=0;
    timeCount++;
    if(timeCount>=9)
    {
        timerStay->stop();
        timerClose->start(200);
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
}

void Notice::leaveEvent(QEvent *)
{
    isEnter = false;
}

void Notice::setMsg(QString szMsg)
{
    ui->label_Notice->setText(szMsg);
}

void Notice::ShowText(QString szMsg)
{
    ui->label_Notice->setText(szMsg);
    show();
}
