#-------------------------------------------------
#
# Project created by QtCreator 2018-08-15T19:40:52
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TwoPisSettings2
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    controller.cpp \
    sshcontroller.cpp \
    pingcontroller.cpp \
    cameraip.cpp \
    pngwidget.cpp

HEADERS += \
        mainwindow.h \
    controller.h \
    sshcontroller.h \
    pingcontroller.h \
    CameraIp.h \
    pngwidget.h

FORMS += \
        mainwindow.ui

CONFIG += c++11

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../usr/lib/x86_64-linux-gnu/release/ -lssh2
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../usr/lib/x86_64-linux-gnu/debug/ -lssh2
else:unix: LIBS += -L/usr/lib/x86_64-linux-gnu/ -lssh2

DEPENDPATH += $$PWD/../../../usr/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../usr/lib/x86_64-linux-gnu/release/libssh2.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../usr/lib/x86_64-linux-gnu/debug/libssh2.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../usr/lib/x86_64-linux-gnu/release/ssh2.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../usr/lib/x86_64-linux-gnu/debug/ssh2.lib
else:unix: PRE_TARGETDEPS += /usr/lib/x86_64-linux-gnu/libssh2.a
