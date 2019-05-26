#-------------------------------------------------
#
# Project created by QtCreator 2019-05-26T08:13:34
#
#-------------------------------------------------

#c++11 support
CONFIG += c++11

QT       +=  core widgets gui

TARGET = PhotoSort
TEMPLATE = app
RC_FILE	 = icon.rc

SOURCES += main.cpp\
        MainWindow.cpp \
    SearchPanel.cpp

HEADERS  += MainWindow.h \
    SearchPanel.h

FORMS    += MainWindow.ui \
    SearchPanel.ui

#include cppCORE library
INCLUDEPATH += $$PWD/../../src/cppCORE
LIBS += -L$$PWD/../../bin -lcppCORE

#include cppGUI library
INCLUDEPATH += $$PWD/../../src/cppGUI
LIBS += -L$$PWD/../../bin -lcppGUI

#copy EXE to bin folder
DESTDIR = $$PWD/../../bin

RESOURCES += \
    resources.qrc

DISTFILES += \
    TODO.txt
