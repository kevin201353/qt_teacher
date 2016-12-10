#ifndef NOTICE_H
#define NOTICE_H

#include <QWidget>
#include <QPoint>
#include <QTimer>

namespace Ui {
class Notice;
}

class Notice : public QWidget
{
    Q_OBJECT

public:
    explicit Notice(QWidget *parent = 0);
    ~Notice();
public:
    void setMsg(QString szMsg);
private:
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
private slots:
    void myMove();
    void myStay();
    void myClose();
    void ShowText(QString szMsg);
private:
    Ui::Notice *ui;
    QTimer *timerShow;
    QTimer *timerStay;
    QTimer *timerClose;
    bool isEnter;
    QPoint normal_Point;
    QString m_szMsg;
};

#endif // NOTICE_H
