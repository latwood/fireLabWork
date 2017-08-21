TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    WindNinjaRun.cpp \
    readConfigFile.cpp \
    WindNinjaAutomationOptions.cpp \
    WindNinjaOptions.cpp

HEADERS += \
    WindNinjaRun.h \
    WindNinjaOptions.h \
    readConfigFile.h \
    WindNinjaAutomationOptions.h

