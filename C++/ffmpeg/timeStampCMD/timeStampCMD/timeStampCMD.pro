TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    generateVideoTimes.cpp \
    readFramesFile.cpp \
    readTimeStampsFile.cpp \
    outputCMDfileForAllTimeStamps.cpp \
    outputCMDfileForOriginalVideoFrames.cpp \
    readVideoTimesFile.cpp \
    adjustTimeStamps.cpp

HEADERS += \
    generateVideoTimes.h \
    readFramesFile.h \
    readTimeStampsFile.h \
    outputCMDfileForAllTimeStamps.h \
    outputCMDfileForOriginalVideoFrames.h \
    readVideoTimesFile.h \
    adjustTimeStamps.h
