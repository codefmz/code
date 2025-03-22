QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

TARGET = zipcode
TEMPLATE = app

PRE_TARGETDEPS += ../bin/libbase.a

LIBS += -L../bin -lbase

INCLUDEPATH += ../base

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ItemDelegate.cpp \
    MainWindow.cpp \
    ProxyModel.cpp \
    StandardTableModel.cpp \
    TableModel.cpp \
    UniqueProxyModel.cpp \
    ZipCodeItem.cpp \
    main.cpp

HEADERS += \
    Base.h \
    ItemDelegate.h \
    MainWindow.h \
    ProxyModel.h \
    Spinbox.h \
    StandardTableModel.h \
    TableModel.h \
    UniqueProxyModel.h \
    ZipCodeItem.h

FORMS +=

DEFINES += CUSTOM_MODEL

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=
