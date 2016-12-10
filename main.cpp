#include "widget.h"
#include <QApplication>
#include <QSettings>
#include <QDir>
void appAutoRun(bool bAutoRun)
{
    //HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Run
    QSettings  reg("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",QSettings::NativeFormat);
    if (bAutoRun)
    {
         QString strAppPath = QDir::toNativeSeparators(QCoreApplication::applicationFilePath());
        //strAppPath.replace(QChar('/'),QChar('\\'),Qt::CaseInsensitive);
        reg.setValue("TeacherRel",strAppPath);
    }
    else
    {
        reg.setValue("TeacherRel","");
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    appAutoRun(true);
    Widget w;
    w.show();
    return a.exec();
}
