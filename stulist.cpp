#include "stulist.h"
#include "ui_stulist.h"
#include <QStandardItemModel>
#include <QStandardItem>
#include <QHeaderView>
#include <QIcon>
#include "cmytableview.h"
#include "widget.h"
#include <QScrollBar>
#include <QMutexLocker>

extern QList<StruInfo> g_stu2List;
extern QList<StruInfo> g_handupList;
extern NetConfig g_config;
//QMutex g_mutexlist;
stulist::stulist(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::stulist)
{
    ui->setupUi(this);
    //setAttribute(Qt::WA_TranslucentBackground, true);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setMouseTracking(true);
    ui->btnStuClose->setStyleSheet("QPushButton{border-image: url(:/images/x.png);}"
                            "QPushButton:pressed{border-image: url(:/images/x.png);}");
    connect(ui->btnStuClose, SIGNAL(clicked(bool)), this, SLOT(exit_widget()));
    ui->stutableView->horizontalHeader()->hide();
    m_ntype = 0;
}

stulist::~stulist()
{
    delete ui;
}

void stulist::mouseMoveEvent(QMouseEvent *event)
{
    if( event->buttons().testFlag(Qt::LeftButton) && m_moving)
    {
      this->move(this->pos() + (event->globalPos() - m_LastMousePosition));
      m_LastMousePosition = event->globalPos();
    }
}

void stulist::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_moving = true;
        m_LastMousePosition = event->globalPos();
    }
}

void stulist::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_moving = false;
    }
}

void stulist::exit_widget()
{
    if (m_ntype == 1)
        settype(0);
    close();
}

void stulist::settype(short ntype)
{
    if (ntype == 0)
    {
      ui->title_label->setText("举手列表");
    }
    else if (ntype == 1)
    {
      ui->title_label->setText("学生列表");
    }
    m_ntype = ntype;
    ui->stutableView->clearSpans();
    ui->stutableView->updateGeometry();
    ui->stutableView->SetType(ntype);
    ui->stutableView->setColumnWidth(0,  522);
    ui->stutableView->setColumnWidth(1, 60);
    Thread * pthread = ((Widget *)m_pWidget)->GetThread();
    DataThread *pdataThread = ((Widget *)m_pWidget)->GetDataThread();
    pthread->settype("stu", ntype);
    pdataThread->settype("stu", ntype);
    QList<QString> msglist;
    QString str = g_config.protocol;
    str += m_addrserver;
    if (ntype == 0)
    {
        str += "/service/classes/list_handupstu";
        msglist.append(str);
    }else if (ntype == 1)
    {
        str += "/service/classes/list_stu";
        msglist.append(str);
    }
    //m_szMac = "00:1a:4a:16:01:57";
    QString data = "vmMac=";
    data += m_szMac;
    msglist.append(data);  //MAC:38:2C:4A:B4:B6:F8
    pthread->setMessage(msglist);
}

QObject* stulist::GetList()
{
    return (QObject *)ui->stutableView;
}
