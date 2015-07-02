#-------------------------------------------------
#
# Project created by QtCreator 2015-07-02T00:03:53
#
#-------------------------------------------------

QT       += core gui svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = analog_clock
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    clockview.cpp

HEADERS  += mainwindow.h \
    clockview.h

FORMS    += mainwindow.ui

RESOURCES += \
    svgclock.qrc
