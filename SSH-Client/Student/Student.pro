#-------------------------------------------------
#
# Project created by QtCreator 2014-12-25T12:18:29
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Student
TEMPLATE = lib
CONFIG += dynamiclib
CONFIG += C++11
QMAKE_CXXFLAGS += -std=c++11

SOURCES += \
    Copyer.cpp \
    Initialization.cpp \
    mainwindow.cpp \
    LocalFile.cpp \
    LocalDirectory.cpp

HEADERS += \
    Copyer.h \
    mainwindow.h \
    LocalFile.h \
    LocalDirectory.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

INCLUDEPATH += $$PWD/../julkinen/harjoitustyo/Staff
DEPENDPATH += $$PWD/../julkinen/harjoitustyo/Staff

FORMS += \
    mainwindow.ui
