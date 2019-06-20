#-------------------------------------------------
#
# Project created by QtCreator 2019-05-26T08:13:34
#
#-------------------------------------------------

#c++11 support
CONFIG += c++11

QT       +=  core widgets gui xml xmlpatterns printsupport

TARGET = RecipeStore
TEMPLATE = app
RC_FILE	 = icon.rc

SOURCES += main.cpp\
        MainWindow.cpp \
    SearchPanel.cpp \
    RecipeCollection.cpp \
    Recipe.cpp

HEADERS  += MainWindow.h \
    SearchPanel.h \
    RecipeCollection.h \
    Recipe.h

FORMS    += MainWindow.ui \
    SearchPanel.ui

#include cppCORE library
INCLUDEPATH += $$PWD/../../src/cppCORE
LIBS += -L$$PWD/../../bin -lcppCORE

#include cppXML library
INCLUDEPATH += $$PWD/../../src/cppXML
LIBS += -L$$PWD/../../bin -lcppXML

#include cppGUI library
INCLUDEPATH += $$PWD/../../src/cppGUI
LIBS += -L$$PWD/../../bin -lcppGUI

#copy EXE to bin folder
DESTDIR = $$PWD/../../bin

RESOURCES += \
    resources.qrc

DISTFILES += \
    TODO.txt
