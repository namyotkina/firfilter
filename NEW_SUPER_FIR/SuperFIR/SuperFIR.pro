#-------------------------------------------------
#
# Project created by QtCreator 2017-07-17T13:38:28
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = SuperFIR
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    qcustomplot.cpp

HEADERS  += widget.h \
    qcustomplot.h \
    fir_filter.h
