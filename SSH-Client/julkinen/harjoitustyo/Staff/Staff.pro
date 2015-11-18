#-------------------------------------------------
#
# Project created by QtCreator 2014-12-25T12:18:00
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Staff
TEMPLATE = app
CONFIG += C++11
QMAKE_CXXFLAGS += -std=c++11

win32 : NOSSH = true

SOURCES += main.cpp\
    virtualconnection.cpp


HEADERS  += DateTime.h \
    CopyerInterface.h \
    ConnectionInterface.h \
    DirectoryEntryInterface.h \
    FileInterface.h \
    DirectoryInterface.h \
    SymbolicLinkInterface.h \
    DoxygenInfo.h \
    VirtualDirectory.h \
    VirtualFile.h \
    virtualconnection.h

isEmpty(NOSSH) {
    SOURCES +=  sftpfileentry.cpp \
        sftpfile.cpp \
        sftpdirectory.cpp \
        sftpconnection.cpp \
        sftplink.cpp \
        loginwindow.cpp \
        loginform.cpp \
        sshconnection.cpp \
        connectingdialog.cpp \
        passworddialog.cpp

    HEADERS += sftpunknown.h \
        sftplink.h \
        loginwindow.h \
        Initialization.h \
        loginform.h \
        sshconnection.h \
        connectingdialog.h \
        passworddialog.h \
        sftpfileentry.h \
        sftpfile.h \
        sftpdirectory.h \
        sftpconnection.h

    FORMS    += loginwindow.ui \
        loginform.ui \
        connectingdialog.ui \
        passworddialog.ui

    QMAKE_CXXFLAGS += -DUSE_SSH
}


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../Student/release/ -lStudent
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../Student/debug/ -lStudent
else:unix: LIBS += -L$$OUT_PWD/../../../Student/ -lStudent

DEPENDPATH += $$PWD/../Student

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += libssh2
