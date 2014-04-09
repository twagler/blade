TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11
LIBS += -pthread

SOURCES += main.cpp \
    CalcWaypoint.cpp \
    gps.cpp \
    motordriver.cpp \
    mower.cpp \
    Print.cpp \
    Ultra.cpp \
    WaypointNavigation.cpp

OTHER_FILES += \
    README.md

HEADERS += \
    gps.h \
    motordriver.h \
    mower.h

