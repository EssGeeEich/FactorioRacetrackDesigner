#-------------------------------------------------
#
# Project created by QtCreator 2019-01-27T14:31:19
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FactorioRacetrackDesigner
TEMPLATE = app
VERSION = 0.3.4

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    qdragbutton.cpp \
    qfactoriotrackeditor.cpp \
    qfactoriozoomablegraphicsview.cpp \
    qfactorioitem_startingarea.cpp \
    qfactorioitem_checkpoint.cpp \
    qfactorioconstants.cpp \
    qserializable.cpp \
    qfactoriobasicitem.cpp \
    qfactorioitem_checkpoint_gfx.cpp \
    qfactorioitem_startingarea_gfx.cpp \
    qfactorioexporter.cpp

HEADERS += \
        mainwindow.h \
    qdragbutton.h \
    qfactoriotrackeditor.h \
    qfactoriozoomablegraphicsview.h \
    qfactorioitem_startingarea.h \
    qfactorioitem_checkpoint.h \
    qfactorioconstants.h \
    qfactoriobasicitem.h \
    qserializable.h \
    qcallbackitem.h \
    qfactorioitem_checkpoint_gfx.h \
    qfactorioitem_startingarea_gfx.h \
    qfactorioexporter.h

FORMS += \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES +=
