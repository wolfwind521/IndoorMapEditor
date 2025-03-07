#-------------------------------------------------
#
# Project created by QtCreator 2014-11-22T10:30:37
#
#-------------------------------------------------

QT       += core gui printsupport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = IndoorMapEditor
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    core/building.cpp \
    core/floor.cpp \
    gui/documentview.cpp \
    core/polygonfeature.cpp \
    io/jsonloader.cpp \
    io/iomanager.cpp \
    core/pubpoint.cpp \
    gui/scenemodel.cpp \
    gui/propertyview.cpp \
    tool/abstracttool.cpp \
    tool/toolmanager.cpp \
    tool/polygontool.cpp \
    core/scene.cpp \
    tool/selecttool.cpp \
    tool/pubpointtool.cpp \
    gui/handle.cpp \
    cmd/addenittycommand.cpp \
    core/imagelayer.cpp \
    gui/propviewfloor.cpp \
    gui/propviewbuilding.cpp \
    tool/splittool.cpp \
    tool/mergetool.cpp \
    tool/scaletool.cpp \
    math/gdiam.cpp \
    tool/edittool.cpp \
    core/room.cpp \
    gui/propviewroom.cpp \
    core/feature.cpp \

HEADERS  += mainwindow.h \
    core/building.h \
    core/floor.h \
    room.h \
    gui/documentview.h \
    core/polygonfeature.h \
    io/jsonloader.h \
    io/iomanager.h \
    core/pubpoint.h \
    gui/scenemodel.h \
    gui/propertyview.h \
    tool/abstracttool.h \
    tool/toolmanager.h \
    tool/polygontool.h \
    core/scene.h \
    tool/selecttool.h \
    tool/pubpointtool.h \
    gui/handle.h \
    cmd/addenittycommand.h \
    core/imagelayer.h \
    gui/propviewfloor.h \
    gui/propviewbuilding.h \
    tool/splittool.h \
    tool/mergetool.h \
    tool/scaletool.h \
    math/gdiam.hpp \
    tool/edittool.h \
    core/room.h \
    gui/propviewroom.h \
    core/feature.h \

FORMS    += mainwindow.ui

TRANSLATIONS = indoormap_zh_cn.ts\
                indoormap_en.ts

RESOURCES += \
    IndoorMapEditor.qrc
