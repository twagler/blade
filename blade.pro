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
    Ultra.cpp \
    WaypointNavigation.cpp \
    motordriver.cpp \
    serial.cpp

HEADERS += \
    gps.h \
    motordriver.h \
    mower.h

OTHER_FILES += \
    README.md \
    input.txt

