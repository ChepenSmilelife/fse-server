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
    databaseconfiguredialog.cpp \
    sqldialog.cpp \
    sqlhelper.cpp \
    md5.c \
    fseserver.cpp \
    fsethread.cpp \
    socketerrorstring.cpp

HEADERS  += mainwindow.h \
    databaseconfiguredialog.h \
    sqldialog.h \
    sqlhelper.h \
    qmd5.h \
    md5.h \
    fseserver.h \
    fsethread.h \
    socketerrorstring.h \
    fseenum.h

FORMS    += mainwindow.ui \
    databaseconfiguredialog.ui \
    sqldialog.ui

RESOURCES +=
