#-------------------------------------------------
#
# Project created by QtCreator 2016-12-10T17:40:29
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = samp6_2
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qwdialogsize.cpp \
    qwdialogheaders.cpp \
    qwdialoglocate.cpp

HEADERS  += mainwindow.h \
    qwdialogsize.h \
    qwdialogheaders.h \
    qwdialoglocate.h

FORMS    += mainwindow.ui \
    qwdialogsize.ui \
    qwdialogheaders.ui \
    qwdialoglocate.ui

RESOURCES += \
    res.qrc


msvc:QMAKE_CXXFLAGS += -execution-charset:utf-8
msvc:QMAKE_CXXFLAGS += -source-charset:utf-8
