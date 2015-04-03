#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>    // needed for serial port
#include <ctype.h>      // needed for isalpha(), etc...
#include <string.h>     // needed for memsetc++

class SerialPort
{
public:
    SerialPort();
    SerialPort(const char*,int);
    int serial_fd;
};

#endif // SERIALPORT_H
