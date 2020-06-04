QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
CONFIG += resources_big

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    choose.cpp \
    databasehelper.cpp \
    forget.cpp \
    login.cpp \
    main.cpp \
    mainwindow.cpp \
    music.cpp \
    play.cpp \
    playworker.cpp \
    rank.cpp \
    register.cpp \
    smtp.cpp \
    time_thread.cpp
    playworker.cpp

HEADERS += \
    choose.h \
    databasehelper.h \
    forget.h \
    login.h \
    mainwindow.h \
    music.h \
    play.h \
    playworker.h \
    rank.h \
    register.h \
    smtp.h \
    time_thread.h
    playworker.h

FORMS += \
    choose.ui \
    forget.ui \
    login.ui \
    mainwindow.ui \
    play.ui \
    rank.ui \
    register.ui

QT += sql

QT += multimedia

QT += testlib

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    media.qrc \
    pic.qrc \
    test.qrc
