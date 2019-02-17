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
    pngwidget.cpp \
    logger.cpp \
    calibwidget.cpp \
    calibcontroller.cpp \
    constants.cpp \
    settingswidget.cpp \
    camerasearchwidget.cpp \
    ipwidget.cpp \
    sizeeditwidget.cpp \
    imageviewer.cpp \
    shotswidget.cpp \
    shotwidget.cpp

HEADERS += \
        mainwindow.h \
    controller.h \
    sshcontroller.h \
    pingcontroller.h \
    CameraIp.h \
    pngwidget.h \
    position.h \
    logger.h \
    calibwidget.h \
    calibcontroller.h \
    settingswidget.h \
    camerasearchwidget.h \
    ipwidget.h \
    sizeeditwidget.h \
    imageviewer.h \
    shotswidget.h \
    shotwidget.h

FORMS += \
        mainwindow.ui

CONFIG += c++11

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../usr/lib/x86_64-linux-gnu/release/ -lssh2
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../usr/lib/x86_64-linux-gnu/debug/ -lssh2
else:unix: LIBS += -L/usr/lib/x86_64-linux-gnu/ -L/usr/local/lib/ -lssh2 -lopencv_calib3d -lopencv_core -lopencv_highgui -lopencv_imgcodecs

DEPENDPATH += $$PWD/../../../usr/include


