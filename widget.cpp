#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QColor>
#include <QGraphicsDropShadowEffect>
#include <QDesktopWidget>
#include <QRect>
#include "log.h"
#include <QMessageBox>
#include "global.h"
#include <QNetworkInterface>
#include <QList>
#include <QFile>
#include "myhttp.h"

#define PORT 5555
#define FTP_PATH "C:\\ftp_upload\\"

QMap<QString, QObject *> g_mapObject;

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    //setAttribute(Qt::WA_TranslucentBackground, true);
    setWindowFlags( Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    QDesktopWidget* desktopWidget = QApplication::desktop();
    QRect deskRect = desktopWidget->availableGeometry();
    setGeometry(QRect((deskRect.width() - width()) / 2, 4 - height(), width(), height()));
    setMouseTracking(true);

    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setOffset(3, 3);
    shadow->setColor(QColor(63, 63, 63, 150));
    shadow->setBlurRadius(8);
    ui->class_room->setText("班级1");
    ui->vm_name->setText("教室1");
    ui->raiseHandPushButton->setStyleSheet("QPushButton{border-image: url(:/images/hand_nor.png);}"
                               "QPushButton:pressed{border-image: url(:/images/hand_press.png);}");
    ui->btn_broadcast->setStyleSheet("QPushButton{border-image: url(:/images/demo_nor.png);}"
                                "QPushButton:pressed{border-image: url(:/images/demo_press.png);}");
    ui->btn_student->setStyleSheet("QPushButton{border-image: url(:/images/student_nor.png);}"
                            "QPushButton:pressed{border-image: url(:/images/student_press.png);}");
    ui->btn_exit->setStyleSheet("QPushButton{border-image: url(:/images/exit_nor.png);}"
                            "QPushButton:pressed{border-image: url(:/images/exit_press.png);}");
    //signal
    connect(ui->btn_exit, SIGNAL(clicked(bool)), this, SLOT(exit_widget()));
    stulistWidget = NULL;
    stulistWidget = new stulist();
    connect(ui->btn_student, SIGNAL(clicked(bool)), this, SLOT(on_stu_clicked()));
    connect(ui->btn_broadcast, SIGNAL(clicked(bool)), this, SLOT(on_demo_clicked()));
    //get host mac
    m_strMac = getHostMacAddress();
    stulistWidget->setmac(m_strMac);
    stulistWidget->setaddress(SERVICE_ADDRESS);
    stulistWidget->setWidget(this);

    g_mapObject["widget"] = this;
    g_mapObject["stulist"] = stulistWidget;
    m_pthread = NULL;
    m_pDataThread = NULL;
    readstulistfromserver();
    procesdata();
    stulistWidget->settype(0);
    g_mapObject["DataThread"] = m_pDataThread;
    m_pthreadx2 = NULL;
    getclassinfo();
    m_pDataThread2 = NULL;
    m_pDataThread2 = new DataThread();
    m_pDataThread2->settype(3);
    m_pDataThread2->start();
}

Widget::~Widget()
{
    if (m_pthread)
    {
        m_pthread->stop();
        delete m_pthread;
        m_pthread = NULL;
    }
    if (m_pDataThread)
    {
        m_pDataThread->stop();
        delete m_pDataThread;
        m_pDataThread = NULL;
    }

    if (stulistWidget)
    {
        delete stulistWidget;
        stulistWidget = NULL;
    }
    if (m_pthreadx2)
    {
        m_pthreadx2->stop();
        delete m_pthreadx2;
        m_pthreadx2 = NULL;
    }
    if (m_pDataThread2)
    {
        m_pDataThread2->stop();
        delete m_pDataThread2;
        m_pDataThread2 = NULL;
    }
    delete ui;
}

void Widget::enterEvent(QEvent *)
{
    move(x(), 0);
}

void Widget::leaveEvent(QEvent *)
{
    move(x(), 4 - height());
}

void Widget::on_raiseHandPushButton_clicked()
{
    //获取举手学生列表
    stulistWidget->settype(0);
    stulistWidget->show();
}

void Widget::exit_widget()
{
    //MESSAGEBOX("this is exit_button.");
    close();
    //qApp->exit();
}

void Widget::on_stu_clicked()
{
    stulistWidget->settype(1);
    stulistWidget->show();
}

QString Widget::getHostMacAddress()
{
    QList<QNetworkInterface> nets = QNetworkInterface::allInterfaces();// 获取所有网络接口列表
    int nCnt = nets.count();
    QString strMacAddr = "";
    for(int i = 0; i < nCnt; i ++)
    {
        // 如果此网络接口被激活并且正在运行并且不是回环地址，则就是我们需要找的Mac地址
        if(nets[i].flags().testFlag(QNetworkInterface::IsUp) && nets[i].flags().testFlag(QNetworkInterface::IsRunning) && !nets[i].flags().testFlag(QNetworkInterface::IsLoopBack))
        {
            strMacAddr = nets[i].hardwareAddress();
            break;
        }
    }
    return strMacAddr;
}

void Widget::on_demo_clicked()
{
    //MESSAGEBOX("this is demo.", this);
    QString url = HTTP_URL_HEAD;
    url += SERVICE_ADDRESS;
    url += "/service/classes/show";
    QString data;
    data = "vmMac=";
    data += m_strMac;
    data += "&";
    data += "type=teacher";
    data += "&";
    data += "onOff=true";

    myHttp *http = new myHttp;
    if (http)
    {
        QString str = "demo clicked : ";
        str += url;
        str += "------";
        str += data;
        writeLogFile(QtDebugMsg, str);
        if (!http->Post(url, data))
        {
            writeLogFile(QtDebugMsg, "teacher demo failed.");
        }
        delete http;
        http = NULL;
    }
}

void Widget::getclassinfo()
{
    QList<QString> msglist;
    QString str = HTTP_URL_HEAD;
    str += SERVICE_ADDRESS;
    str += "/service/desktops/classinfo";
    msglist.append(str);
    m_strMac = "00:1a:4a:16:01:56";
    QString data = "vmMac=";
    data += m_strMac;
    msglist.append(data);  //MAC:38:2C:4A:B4:B6:F8
    m_pthreadx2 = new Thread();
    m_pthreadx2->settype(3);
    m_pthreadx2->setMessage(msglist);
    m_pthreadx2->start();
}

void Widget::setvmclass(QString str1, QString str2)
{
    ui->vm_name->setText(str1);
    ui->class_room->setText(str2);
}

void Widget::SetNoticeMsg2(QString szMsg)
{
    //Notice 消息框
    emit ShowNotice(szMsg);
}

void Widget::readstulistfromserver()
{
    m_pthread = new Thread();
    m_pthread->start();
}

void Widget::procesdata()
{
    m_pDataThread = new DataThread();
    connect(m_pDataThread, SIGNAL(NoticeShow(QString)), this, SLOT(NoticeMsg(QString)));
    m_pDataThread->start();
}

void Widget::NoticeMsg(QString szMsg)
{
    Notice notice;
    emit ShowNotice(szMsg);
}

