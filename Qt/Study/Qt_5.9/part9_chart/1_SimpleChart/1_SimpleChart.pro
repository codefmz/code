#-------------------------------------------------
#
# Project created by QtCreator 2017-01-15T01:06:50
#
#-------------------------------------------------

QT       += core gui

QT       += charts


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = samp9_1
TEMPLATE = app

msvc:QMAKE_CXXFLAGS += -execution-charset:utf-8
msvc:QMAKE_CXXFLAGS += -source-charset:utf-8

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
