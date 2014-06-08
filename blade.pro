TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11
LIBS += -pthread

SOURCES += main.cpp \
    CalcWaypoint.cpp \
    gps.cpp \
    mower.cpp \
    Print.cpp \
    WaypointNavigation.cpp \
    motordriver.cpp \
    joystick.cpp \
    ManualControl.cpp \
    controlserver.cpp \
    serial.cpp \
    ultrasonic.cpp

HEADERS += \
    gps.h \
    motordriver.h \
    mower.h \
    joystick.h \
    controlserver.h \
    serial.h \
    ultrasonic.h

OTHER_FILES += \
    README.md \
    input.txt

