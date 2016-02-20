QT += core
QT += positioning
QT += network
QT -= gui

CONFIG += c++11

TARGET = blade-Qt
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    maincontroller.cpp \
    gps_receiver.cpp \
    networkserver.cpp \
    networkserverthread.cpp \
    sabertoothdriver.cpp \
    ultrasonicsensor.cpp \
    navigation.cpp \
    mappingservice.cpp

HEADERS += \
    maincontroller.h \
    gps_receiver.h \
    networkserver.h \
    networkserverthread.h \
    sabertoothdriver.h \
    ultrasonicsensor.h \
    navigation.h \
    mappingservice.h
