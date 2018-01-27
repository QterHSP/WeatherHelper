#-------------------------------------------------
#
# Project created by QtCreator 2017-10-25T22:18:54
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WeatherHelper
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ctrlpanel.cpp

HEADERS  += mainwindow.h \
    ctrlpanel.h

FORMS    += mainwindow.ui \
    ctrlpanel.ui

RESOURCES += \
    resource.qrc

DISTFILES +=
