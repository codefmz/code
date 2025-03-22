QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

TARGET = timelog
TEMPLATE = app

PRE_TARGETDEPS += ../bin/libbase.a

LIBS += -L../bin -lbase

INCLUDEPATH += ../base

DEFINES += CUSTOM_MODEL

SOURCES += \
    RichTextDelegate.cpp \
    RichTextLineEdit.cpp \
    StandardItem.cpp \
    StandardTreeModel.cpp \
    TaskItem.cpp \
    TreeModel.cpp \
    main.cpp \
    MainWindow.cpp

HEADERS += \
    MainWindow.h \
    RichTextDelegate.h \
    Base.h \
    RichTextLineEdit.h \
    StandardItem.h \
    StandardTreeModel.h \
    TaskItem.h \
    TreeModel.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    treeview.qrc

win32: LIBS += -L$$PWD/lib/ -ladd

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/lib/add.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/lib/libadd.a
