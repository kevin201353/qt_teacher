#include "sendfile.h"
#include "ui_sendfile.h"
#include <QMimeData>
#include <QDesktopWidget>
#include <QRect>
#include <QPoint>
#include <QFile>

#define FTP_PATH "C:\\ftp_upload\\"

SendFile::SendFile(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SendFile)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_TranslucentBackground, true);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    QDesktopWidget* desktopWidget = QApplication::desktop();
    QRect deskRect = desktopWidget->availableGeometry();
    QPoint bottomright = deskRect.bottomRight();
    setGeometry(QRect(bottomright.x()-width()-10, bottomright.y()-height()-10, width(), height()));

    ui->label_2->setStyleSheet("background-color: rgba(42, 116, 190, 150)");
    ui->label->setStyleSheet("border-image: url(:/images/file_receiver.png);");
    timer.setSingleShot(true);
}

SendFile::~SendFile()
{
    delete ui;
}

void SendFile::closeThis()
{
    emit finished();
    ui->label->setStyleSheet("border-image: url(:/images/file_receiver.png);");
    close();
}

void SendFile::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasFormat("text/uri-list"))
        event->acceptProposedAction();
}

void SendFile::dropEvent(QDropEvent *event)
{
    QList<QUrl> urls = event->mimeData()->urls();
    if(urls.isEmpty())
        return;

    QString srcFilePath = urls.first().toLocalFile();
    QFile canOpen(srcFilePath);
    if(canOpen.open(QIODevice::ReadOnly) == false)
        return;
    fileName.clear();
    fileName = srcFilePath.section('/', -1);
    QString destFilePath = FTP_PATH + fileName;
    srcFilePath.replace("/", "\\");
    QFile::copy(srcFilePath, destFilePath);

    ui->label->setStyleSheet("border-image: url(:/images/sendfile_success.png);");
    timer.start(3000);
    connect(&timer, SIGNAL(timeout()), this, SLOT(closeThis()));
}
