#ifndef SETSERVERIP_H
#define SETSERVERIP_H

#include <QWidget>
#include <QString>

namespace Ui {
class SetServerIP;
}

class SetServerIP : public QWidget
{
    Q_OBJECT

public:
    explicit SetServerIP(QWidget *parent = 0);
    ~SetServerIP();

    QString ServerIP;

signals:
    void finished();

private slots:
    void on_pushButton_clicked();

private:
    Ui::SetServerIP *ui;
};

#endif // SETSERVERIP_H
