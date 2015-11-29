#-------------------------------------------------
#
# Project created by QtCreator 2015-04-27T10:42:49
#
#-------------------------------------------------

QT       -= core gui

TARGET = mallikoodia-rinn2015kirjasto-QtCreator
CONFIG   += console c++11 thread
CONFIG   -= app_bundle

TEMPLATE = app

BOOST = /usr/local/lang/boost-1.57
RINN2015LIB = /home/rinn/pub/2015

INCLUDEPATH += $$RINN2015LIB
LIBS += -L$$RINN2015LIB -lrinn2015
LIBS += -L$$BOOST/lib -Wl,-rpath,$$BOOST/lib -lboost_thread -lboost_thread-mt -lboost_system -lboost_system-mt


SOURCES += \
    main.cc \
    saie.cc

HEADERS += \
    saie.h
