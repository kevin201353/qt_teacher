#include "setserverip.h"
#include "ui_setserverip.h"
#include <QRegExp>

SetServerIP::SetServerIP(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SetServerIP)
{
    ui->setupUi(this);

    QRegExp RegIPLineEdit("(\\d{1,2}|1\\d\\d|2[0-4]\\d|25[0-5])\\."
                          "(\\d{1,2}|1\\d\\d|2[0-4]\\d|25[0-5])\\."
                          "(\\d{1,2}|1\\d\\d|2[0-4]\\d|25[0-5])\\."
                          "(\\d{1,2}|1\\d\\d|2[0-4]\\d|25[0-5])");
    ui->lineEditIP->setValidator(new QRegExpValidator(RegIPLineEdit, this));
    ui->emptyTextEdit->hide();
    ui->invalidTextEdit->hide();
    ui->pushButton->setShortcut((tr("return")));
}

SetServerIP::~SetServerIP()
{
    delete ui;
}

void SetServerIP::on_pushButton_clicked()
{
    ServerIP.clear();
    ServerIP = ui->lineEditIP->text();
    if(ServerIP.isEmpty())
    {
        ui->invalidTextEdit->hide();
        ui->emptyTextEdit->show();
    }
    else if(ServerIP.length() < 7 || ServerIP.count('.') != 3)
    {
        ui->emptyTextEdit->hide();
        ui->invalidTextEdit->show();
    }
    else
    {
        emit finished();
        ui->emptyTextEdit->hide();
        ui->invalidTextEdit->hide();
        close();
    }
}
