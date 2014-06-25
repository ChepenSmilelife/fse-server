#-------------------------------------------------
#
# Project created by QtCreator 2014-06-24T21:00:33
#
#-------------------------------------------------

QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = fse-server
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    databaseconfiguredialog.cpp

HEADERS  += mainwindow.h \
    databaseconfiguredialog.h

FORMS    += mainwindow.ui \
    databaseconfiguredialog.ui

RESOURCES +=
