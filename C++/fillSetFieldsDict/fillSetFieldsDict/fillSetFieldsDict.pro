TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    editSetFieldsDict.cpp \
    Tvalue.cpp \
    Tprofile.cpp \
    generateLinesToAddRefinedMesh.cpp \
    generateLinesToAdd.cpp

HEADERS += \
    editSetFieldsDict.h \
    Tvalue.h \
    Tprofile.h \
    generateLinesToAddRefinedMesh.h \
    generateLinesToAdd.h
