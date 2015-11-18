#-------------------------------------------------
#
# Project created by QtCreator 2015-03-12T15:24:04
#
#-------------------------------------------------

QT       += testlib
QT       -= gui

TARGET = CopyerTest
TEMPLATE = app
CONFIG   += console
CONFIG   -= app_bundle
CONFIG += C++11
QMAKE_CXXFLAGS += -std=c++11

SOURCES += CopyerTest.cpp \
  #  LocalFileTest.cpp \
    ../Student/Copyer.cpp \
    ../Student/LocalFile.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"

INCLUDEPATH += $$PWD/../Student
DEPENDPATH += $$PWD/../Student

INCLUDEPATH += $$PWD/../julkinen/harjoitustyo/Staff
DEPENDPATH += $$PWD/../julkinen/harjoitustyo/Staff

HEADERS += \
    ../Student/Copyer.h \
    ../Student/LocalFile.h
