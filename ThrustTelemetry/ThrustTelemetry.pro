#-------------------------------------------------
#
# Project created by QtCreator 2017-11-13T13:42:15
#
#-------------------------------------------------

QT       += core gui serialport widgets printsupport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ThrustTelemetry
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    serialinitial.cpp \
    serial.cpp \
    datastorage.cpp \
    stringparser.cpp \
    parseddatastorage.cpp \
    plotanddump.cpp \
    qcustomplot.cpp \
    fileinitial.cpp \
    axistag.cpp

HEADERS += \
        mainwindow.h \
    serialinitial.h \
    serial.h \
    datastorage.h \
    stringparser.h \
    parseddatastorage.h \
    plotanddump.h \
    qcustomplot.h \
    fileinitial.h \
    axistag.h

FORMS += \
        mainwindow.ui \
    serialinitial.ui \
    plotanddump.ui \
    fileinitial.ui


