#-------------------------------------------------
#
# Project created by QtCreator 2013-04-01T19:17:56
#
#-------------------------------------------------

QT       += core gui

TARGET = StandaloneEigenfaces
TEMPLATE = app
LIBS += -lgsl
INCLUDEPATH += /usr/include/gsl/
CONFIG += qtimagelib

SOURCES += main.cpp\
        mainwindow.cpp \
    matrix.cpp

HEADERS  += mainwindow.h \
    matrix.h

FORMS    += mainwindow.ui
