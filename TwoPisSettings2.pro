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
        src/main.cpp \
        src/widget/mainwindow.cpp \
    	src/controller/controller.cpp \
    	src/controller/sshcontroller.cpp \
    	src/controller/pingcontroller.cpp \
    	src/model/cameraip.cpp \
        src/model/chessBoard.cpp \
        src/model/constants.cpp \
        src/widget/lib/selectiveImageLabel.cpp \
    	src/logger.cpp \
    	src/widget/calibwidget.cpp \
    	src/controller/calibcontroller.cpp \
    	src/widget/settingswidget.cpp \
    	src/widget/camerasearchwidget.cpp \
    	src/widget/ipwidget.cpp \
    	src/widget/lib/sizeeditwidget.cpp \
    	src/widget/lib/imageviewer.cpp \
    	src/widget/shotswidget.cpp \
    	src/widget/shotwidget.cpp \
    src/controller/cornerssorter.cpp

HEADERS += \
        src/widget/mainwindow.h \
    	src/controller/controller.h \
    	src/controller/sshcontroller.h \
    	src/controller/pingcontroller.h \
    	src/model/cameraip.h \
        src/model/calibData.h \
        src/model/cameraip.h \
        src/model/chessBoard.h \
        src/model/position.h \
        src/widget/lib/selectiveImageLabel.h \
    	src/logger.h \
    	src/widget/calibwidget.h \
    	src/controller/calibcontroller.h \
    	src/widget/settingswidget.h \
    	src/widget/camerasearchwidget.h \
    	src/widget/ipwidget.h \
    	src/widget/lib/sizeeditwidget.h \
    	src/widget/lib/imageviewer.h \
        src/widget/shotswidget.h \
        src/widget/shotwidget.h \
    src/controller/cornerssorter.h

FORMS += \
        mainwindow.ui

CONFIG += c++11

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../usr/lib/x86_64-linux-gnu/release/ -lssh2
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../usr/lib/x86_64-linux-gnu/debug/ -lssh2
else:unix: LIBS += -L/usr/lib/x86_64-linux-gnu/ -L/usr/local/lib/ -lssh2 -lopencv_calib3d -lopencv_core -lopencv_highgui -lopencv_imgcodecs -lopencv_features2d -lopencv_flann -lopencv_imgproc

DEPENDPATH += $$PWD/../../../usr/include 
DEPENDPATH += $$PWD/src

INCLUDEPATH += $$PWD/src $$PWD/src/controller $$PWD/src/model $$PWD/src/widget
