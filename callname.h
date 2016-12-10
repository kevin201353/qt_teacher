#ifndef CALLNAME_H
#define CALLNAME_H

#include <QWidget>
#include <QString>
#include <QLabel>

namespace Ui {
class CallName;
}

class CallName : public QWidget
{
    Q_OBJECT

public:
    explicit CallName(QWidget *parent = 0);
    ~CallName();
    QString name;
    QLabel *classNum;

signals:
    void finished();

private slots:
    void on_pushButtonEnsure_clicked();

private:
    Ui::CallName *ui;
};

#endif // CALLNAME_H
