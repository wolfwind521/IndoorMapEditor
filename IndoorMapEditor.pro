#-------------------------------------------------
#
# Project created by QtCreator 2014-11-22T10:30:37
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = IndoorMapEditor
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    core/building.cpp \
    core/floor.cpp \
    core/funcarea.cpp \
    gui/documentview.cpp \
    core/polygonentity.cpp \
    io/jsonloader.cpp \
    io/iomanager.cpp \
    gui/scenetreemodel.cpp \
    core/mapentity.cpp \
    core/pubpoint.cpp

HEADERS  += mainwindow.h \
    core/building.h \
    core/floor.h \
    funcarea.h \
    core/funcarea.h \
    gui/documentview.h \
    core/polygonentity.h \
    io/jsonloader.h \
    io/iomanager.h \
    gui/scenetreemodel.h \
    core/mapentity.h \
    core/pubpoint.h

FORMS    += mainwindow.ui

RESOURCES += \
    IndoorMapEditor.qrc
