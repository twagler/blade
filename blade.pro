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
    ultrasonic.cpp \
    libSerial/PosixSignalDispatcher.cpp \
    libSerial/SerialPort.cpp \
    libSerial/SerialStream.cc \
    libSerial/SerialStreamBuf.cc \
    libGPIO/libgpio.cpp

HEADERS += \
    gps.h \
    motordriver.h \
    mower.h \
    joystick.h \
    controlserver.h \
    ultrasonic.h \
    libSerial/PosixSignalDispatcher.h \
    libSerial/PosixSignalHandler.h \
    libSerial/SerialPort.h \
    libSerial/SerialStream.h \
    libSerial/SerialStreamBuf.h \
    libgpio.h \
    libGPIO/libgpio.h

OTHER_FILES += \
    README.md \
    input.txt

QMAKE_CXXFLAGS+=-Wno-ignored-qualifiers
