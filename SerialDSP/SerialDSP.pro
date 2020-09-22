#-------------------------------------------------
#
# Project created by QtCreator 2020-09-13T13:21:25
#
#-------------------------------------------------

QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SerialDSP
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qcustomplot.cpp

HEADERS  += mainwindow.h \
    qcustomplot.h \
    fifo.h

FORMS    += mainwindow.ui
