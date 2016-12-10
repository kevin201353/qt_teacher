#include "stulist.h"
#include "ui_stulist.h"

stulist::stulist(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::stulist)
{
    ui->setupUi(this);
}

stulist::~stulist()
{
    delete ui;
}
