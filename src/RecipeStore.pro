TEMPLATE = subdirs
CONFIG += console

#Library targets and depdendencies
SUBDIRS = cppCORE\
        cppGUI\
        RecipeStore

cppGUI.depends = cppCORE
RecipeStore.depends = cppCORE cppGUI
