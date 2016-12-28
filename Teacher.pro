#-------------------------------------------------
#
# Project created by QtCreator 2016-10-26T11:05:20
#
#-------------------------------------------------

QT       += core gui network xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Teacher
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    sendfile.cpp \
    callname.cpp \
    MyGlobalShortCut/MyGlobalShortCut.cpp \
    MyGlobalShortCut/MyWinEventFilter.cpp \
    setserverip.cpp \
    streamparsexml.cpp \
    stulist.cpp \
    mytableview.cpp \
    myhttp.cpp \
    log.cpp \
    message.cpp \
    thread.cpp \
    commjsonpaser.cpp \
    notice.cpp \
    myqlist.cpp

HEADERS  += widget.h \
    sendfile.h \
    callname.h \
    MyGlobalShortCut/MyGlobalShortCut.h \
    MyGlobalShortCut/MyWinEventFilter.h \
    setserverip.h \
    streamparsexml.h \
    log.h \
    stulist.h \
    cmytableview.h \
    global.h \
    myhttp.h \
    thread.h \
    commjsonpaser.h \
    notice.h \
    myqlist.h

FORMS    += widget.ui \
    callname.ui \
    sendfile.ui \
    setserverip.ui \
    stulist.ui \
    notice.ui

RESOURCES += \
    resource.qrc
