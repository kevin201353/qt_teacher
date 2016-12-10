#include "callname.h"
#include "ui_callname.h"
#include <QDesktopWidget>
#include <QRect>
#include <QRegExp>

CallName::CallName(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CallName)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    classNum = ui->classLabel;

    ui->frame->setStyleSheet("border-image: url(:/images/call_name.png);");
    QDesktopWidget* desktopWidget = QApplication::desktop();
    QRect screenRect = desktopWidget->screenGeometry();
    ui->frame->resize(screenRect.width(), screenRect.height());
    ui->groupBox->setAttribute(Qt::WA_TranslucentBackground, true);
    ui->groupBox->setStyleSheet("QGroupBox{border:none;}");
    ui->groupBox->setGeometry(QRect((screenRect.width() - ui->groupBox->width()) / 2,
                                    (screenRect.height() - ui->groupBox->height()) / 2 - 100,
                                    ui->groupBox->width(), ui->groupBox->height()));
    ui->nameTextEdit->hide();
    ui->pushButtonEnsure->setShortcut((tr("return")));
}

CallName::~CallName()
{
    delete ui;
}

void CallName::on_pushButtonEnsure_clicked()
{
    name.clear();
    name = ui->name->text();
    if(name.isEmpty())
        ui->nameTextEdit->show();
    else
    {
        emit finished();
        ui->nameTextEdit->hide();
        close();
    }
}
