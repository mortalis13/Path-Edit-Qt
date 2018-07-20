#-------------------------------------------------
#
# Project created by QtCreator 2015-06-06T07:55:32
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QMAKE_CXXFLAGS += -std=c++11

TARGET = PathEdit
TEMPLATE = app

RC_FILE = resources.rc


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
