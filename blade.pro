TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11
LIBS += -pthread

SOURCES += main.cpp \
    gps.cpp \
    mower.cpp \
    WaypointNavigation.cpp \
    motordriver.cpp \
    joystick.cpp \
    ManualControl.cpp \
    controlserver.cpp \
    ultrasonic.cpp \
    libGPIO/libgpio.cpp \
    NMEAParser/NMEAParser.cpp \
    serialport.cpp \
    lawnmap.cpp \
    quadtree.cpp \
    global.cpp

HEADERS += \
    gps.h \
    motordriver.h \
    mower.h \
    joystick.h \
    controlserver.h \
    ultrasonic.h \
    libgpio.h \
    libGPIO/libgpio.h \
    NMEAParser/NMEAParser.h \
    serialport.h \
    lawnmap.h \
    quadtree.h \
    global.h

OTHER_FILES += \
    README.md \
    input.txt

DISTFILES += \
    Makefile
