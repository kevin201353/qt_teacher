#ifndef STULIST_H
#define STULIST_H

#include <QWidget>

namespace Ui {
class stulist;
}

class stulist : public QWidget
{
    Q_OBJECT

public:
    explicit stulist(QWidget *parent = 0);
    ~stulist();

private:
    Ui::stulist *ui;
};

#endif // STULIST_H
