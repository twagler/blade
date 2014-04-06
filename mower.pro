TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11
LIBS += -pthread

SOURCES += \
    CalcWaypoint.cpp \
    main.cpp \
    mower.cpp \
    Print.cpp \
    Ultra.cpp \
    WaypointNavigation.cpp \
    gps.cpp

HEADERS += \
    mower.h \
    gps.h

OTHER_FILES +=

