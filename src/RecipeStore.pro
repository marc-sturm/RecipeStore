TEMPLATE = subdirs
CONFIG += console

#Library targets and depdendencies
SUBDIRS = cppCORE\
        cppGUI\
        cppXML\
        RecipeStore

cppXML.depends = cppCORE
cppGUI.depends = cppCORE
RecipeStore.depends = cppCORE cppGUI
