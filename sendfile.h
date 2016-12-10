#ifndef SENDFILE_H
#define SENDFILE_H

#include <QWidget>
#include <QFile>
#include <QDropEvent>
#include <QDragEnterEvent>
#include <QTimer>
#include <QString>

namespace Ui {
class SendFile;
}

class SendFile : public QWidget
{
    Q_OBJECT

public:
    explicit SendFile(QWidget *parent = 0);
    ~SendFile();
    QTimer timer;
    QString fileName;
signals:
    void finished();

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);

private slots:
    void closeThis();

private:
    Ui::SendFile *ui;
};

#endif // SENDFILE_H
