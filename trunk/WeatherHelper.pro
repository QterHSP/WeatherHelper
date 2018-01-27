#-------------------------------------------------
#
# Project created by QtCreator 2017-10-25T22:18:54
#
#-------------------------------------------------

QT       += core gui network printsupport

#DEFINES+= QT_NO_DEBUG_OUTPUT

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WeatherHelper
TEMPLATE = app

RESOURCES += \
    resource.qrc


SUBDIRS += \
    Main/WeatherHelper.pro

FORMS += \
    Main/ctrlpanel.ui \
    Main/mainwindow.ui

HEADERS += \
    Main/ctrlpanel.h \
    Main/mainwindow.h \
    qcustomplot/qcustomplot.h

SOURCES += \
    Main/ctrlpanel.cpp \
    Main/main.cpp \
    Main/mainwindow.cpp \
    qcustomplot/qcustomplot.cpp
