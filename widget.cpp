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
#include "streamparsexml.h"
#include "myqlist.h"

#define PORT 5555
#define FTP_PATH "C:\\ftp_upload\\"

QMap<QString, QObject *> g_mapObject;
//NetConfig g_config;
MyQList   g_NoticeList;
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    //读取网络配置文件
    //StreamParseXml netxl;
    //netxl.readNetConfig("netconfig.xml", &g_config);
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
                               "QPushButton:hover{border-image: url(:/images/hand_press.png);}"
                               "QPushButton:pressed{border-image: url(:/images/hand_nor.png);}");
    ui->btn_broadcast->setStyleSheet("QPushButton{border-image: url(:/images/demo_nor.png);}"
                                "QPushButton:hover{border-image: url(:/images/demo_press.png);}"
                                "QPushButton:pressed{border-image: url(:/images/demo_nor.png);}");
    ui->btn_student->setStyleSheet("QPushButton{border-image: url(:/images/student_nor.png);}"
                            "QPushButton:hover{border-image: url(:/images/student_press.png);}"
                            "QPushButton:pressed{border-image: url(:/images/student_nor.png);}");
    ui->btn_exit->setStyleSheet("QPushButton{border-image: url(:/images/exit_nor.png);}"
                            "QPushButton:hover{border-image: url(:/images/exit_press.png);}"
                            "QPushButton:pressed{border-image: url(:/images/exit_nor.png);}");
    //signal
    connect(ui->btn_exit, SIGNAL(clicked(bool)), this, SLOT(exit_widget()));
    stulistWidget = NULL;
    stulistWidget = new stulist();
    connect(ui->btn_student, SIGNAL(clicked(bool)), this, SLOT(on_stu_clicked()));
    connect(ui->btn_broadcast, SIGNAL(clicked(bool)), this, SLOT(on_demo_clicked()));
    //get host mac
    g_mylog.open();
    m_strMac = getHostMacAddress().toLower();
    stulistWidget->setmac(m_strMac);
    stulistWidget->setaddress(SERVICE_ADDRESS);
    stulistWidget->setWidget(this);

    ui->btn_broadcast->setToolTip("演示");
    ui->raiseHandPushButton->setToolTip("举手列表");
    ui->btn_student->setToolTip("学生列表");
    ui->btn_exit->setToolTip("退出");

    g_mapObject["widget"] = this;
    g_mapObject["stulist"] = stulistWidget;
    m_pthread = NULL;
    m_pDataThread = NULL;
    readstulistfromserver();
    procesdata();
    stulistWidget->settype(0);
    g_mapObject["DataThread"] = m_pDataThread;
    //getclassinfo();
    m_bstart = false;
    m_pclassThrd = NULL;
    m_pclassThrd = new classThread;
    m_pclassThrd->setMac(m_strMac);
    m_pclassThrd->start();

    m_NoticeThread = NULL;
    m_NoticeThread = new NoticeThread;
    connect(m_NoticeThread, SIGNAL(SINoticeShow(StruInfo*)), this, SLOT(ShowNoticeXX(StruInfo*)));
    m_NoticeThread->start();
}

Widget::~Widget()
{
    if (m_pthread)
    {
        m_pthread->stop();
        m_pthread->wait();
        delete m_pthread;
        m_pthread = NULL;
    }
    if (m_pDataThread)
    {
        m_pDataThread->stop();
        m_pDataThread->wait();
        delete m_pDataThread;
        m_pDataThread = NULL;
    }

    if (stulistWidget)
    {
        delete stulistWidget;
        stulistWidget = NULL;
    }
    if (m_pclassThrd)
    {
        m_pclassThrd->stop();
        m_pclassThrd->wait();
        delete m_pclassThrd;
        m_pclassThrd = NULL;
    }
    if (m_NoticeThread)
    {
        m_NoticeThread->stop();
        m_NoticeThread->wait();
        delete m_NoticeThread;
        m_NoticeThread = NULL;
    }
    delete ui;
    g_mylog.close();
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
    QString url = HTTP_URL_HEAD;
    url += SERVICE_ADDRESS;
    url += "/service/classes/tec_exit_desktop";
    QString data;
    //m_strMac = "00:1a:4a:16:01:57";
    data = "vmMac=";
    data += m_strMac;
    myHttp *http = new myHttp;
    if (http)
    {
        QString str = "send exit teacher terminal request: ";
        str += url;
        str += "------";
        str += data;
        writeLogFile(QtDebugMsg, str);
        if (!http->Post(url, data))
        {
            writeLogFile(QtDebugMsg, "send exit teacher terminal request failed.");
            delete http;
            http = NULL;
            return;
        }
        QString strTmp;
        str = "send exit teacher terminal -------";
        http->GetData(strTmp);
        str += strTmp;
        writeLogFile(QtDebugMsg, str);
        StreamParseXml parsexml;
        QString result;
        parsexml.readxmlComm(strTmp, "success", result, 0);
        delete http;
        http = NULL;
        if (result == "false")
        {
             writeLogFile(QtDebugMsg, "send exit teacher terminal request failed 2222.");
        }
    }//if
    close();  //test
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
    //m_strMac = "00:1a:4a:16:01:57";
    data = "vmMac=";
    data += m_strMac;
    data += "&";
    data += "type=teacher";
    data += "&";
    bool bDemoRunning = false;
    if (!m_bstart)
    {
        data += "onOff=true";
        m_bstart = true;
    }else
    {
        data += "onOff=false";
        m_bstart = false;
    }
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
            delete http;
            http = NULL;
            return;
        }
        QString strTmp;
        str = "demo clicked -------";
        http->GetData(strTmp);
        str += strTmp;
        writeLogFile(QtDebugMsg, str);
        StreamParseXml parsexml;
        QString result;
        parsexml.readxmlComm(strTmp, "success", result, 0);
        delete http;
        http = NULL;
        if (result == "true" || result == "操作成功")
        {
            bDemoRunning = true;
        }
        if (bDemoRunning && m_bstart)
        {
            ui->btn_broadcast->setStyleSheet("QPushButton{border-image: url(:/images/demo_stop_nor.png);}"
                                          "QPushButton:hover{border-image: url(:/images/demo_stop_nor.png);}"
                                        "QPushButton:pressed{border-image: url(:/images/demo_stop_press.png);}");
        }
        if (bDemoRunning && !m_bstart)
        {
            ui->btn_broadcast->setStyleSheet("QPushButton{border-image: url(:/images/demo_nor.png);}"
                                         "QPushButton:hover{border-image: url(:/images/demo_press.png);}"
                                        "QPushButton:pressed{border-image: url(:/images/demo_nor.png);}");
        }
    }
}

void Widget::getclassinfo()
{
    QString url = HTTP_URL_HEAD;
    url += SERVICE_ADDRESS;
    url += "/service/desktops/classinfo";
    //url += "/service/classes/classinfo";  //test
    //m_strMac = "00:1a:4a:16:01:57";
    QString data = "vmMac=";
    data += m_strMac;
    myHttp http;
    QString strdeg = "getclassinfo : ";
    strdeg += url;
    strdeg += "------";
    strdeg += data;
    writeLogFile(QtDebugMsg, strdeg);
    if (!http.Post(url, data))
    {
        writeLogFile(QtDebugMsg, "getclassinfo failed.");
    }
    QString strBuf;
    http.GetData(strBuf);
    StreamParseXml xmlparse;
    xmlparse.readxmlclass(strBuf);
}

void Widget::setvmclass(QString str1, QString str2)
{
    QString strVm = "桌面：";
    QString strClass = "班级：";
    strVm += str1;
    strClass += str2;
    ui->vm_name->setText(strVm);
    ui->class_room->setText(strClass);
}

void Widget::readstulistfromserver()
{
    m_pthread = new Thread();
    m_pthread->start();
}

void Widget::procesdata()
{
    m_pDataThread = new DataThread();
    connect(m_pDataThread, SIGNAL(NoticeShow(StruInfo*)), this, SLOT(NoticeMsg(StruInfo*)));
    m_pDataThread->start();
}

bool m_bNoticeRunning = false;
void Widget::NoticeMsg(StruInfo* info)
{
//   Notice *pNotice = NULL;
//   if (!m_bNoticeRunning)
//   {
//       pNotice = new Notice;
//       pNotice->printTest();
//       emit ShowNotice(szMsg);
//   }
    g_NoticeList.append(info);
}

static Notice *pNotice = NULL;
void Widget::ShowNoticeXX(StruInfo* info)
{
    if (info != NULL)
    {
        if (!m_bNoticeRunning)
        {
            pNotice = new Notice;
            pNotice->printTest();
            emit ShowNotice(info);
        }else
        {
            pNotice->setMsg(info);
        }
    }
}
