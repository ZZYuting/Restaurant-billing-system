#-------------------------------------------------
#
# Project created by QtCreator 2016-08-08T15:38:58
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ytnew4
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    libcam.cpp \
    fpga.cpp

HEADERS  += mainwindow.h \
    libcam.h \
    fpga.h

FORMS    += mainwindow.ui

#加控制台配置
#CONFIG +=console
INCLUDEPATH += /usr/local/include/opencv/ \
               /usr/local/include/opencv2/ \
               /usr/local/include/
LIBS += /usr/local/lib/libopencv_*
INCLUDEPATH += /home/yuting/include
DEPENDPATH += /home/yuting/include
QMAKE_CXXFLAGS += -std=c++0x
