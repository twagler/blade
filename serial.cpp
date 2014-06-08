#include <termios.h>    /*Enables us to set baud rate for RX/TX seperately*/
#include <fcntl.h>      /*Enables use of flags to modify open(), read(), write() functions*/
#include <unistd.h>     /*Enables use of open(), read(), write()*/
#include <string>
#include <sstream>
#include <iostream>
#include "serial.h"


serial::serial()
{

}

int serial::serial_initialize(const char* port_name)
{
    int fd = my_serial_fd;

    const char *device = port_name;

    fd = open(device, O_RDWR | O_NOCTTY | O_NDELAY);
    if(fd == -1)
        printf( "failed to open port\n" );

    struct termios  config;
    if(!isatty(fd))
    {
        // ... error handling ...
    }
    if(tcgetattr(fd, &config) < 0)
    {
        // ... error handling ...
    }
    //
    // Input flags - Turn off input processing
    // convert break to null byte, no CR to NL translation,
    // no NL to CR translation, don't mark parity errors or breaks
    // no input parity check, don't strip high bit off,
    // no XON/XOFF software flow control
    //
    config.c_iflag &= ~(IGNBRK | BRKINT | ICRNL |
                        INLCR | PARMRK | INPCK | ISTRIP | IXON);
    //
    // Output flags - Turn off output processing
    // no CR to NL translation, no NL to CR-NL translation,
    // no NL to CR translation, no column 0 CR suppression,
    // no Ctrl-D suppression, no fill characters, no case mapping,
    // no local output processing
    //
    // config.c_oflag &= ~(OCRNL | ONLCR | ONLRET |
    //                     ONOCR | ONOEOT| OFILL | OLCUC | OPOST);
    config.c_oflag = 0;
    //
    // No line processing:
    // echo off, echo newline off, canonical mode off,
    // extended input processing off, signal chars off
    //
    config.c_lflag &= ~(ECHO | ECHONL | ICANON | IEXTEN | ISIG);
    //
    // Turn off character processing
    // clear current char size mask, no parity checking,
    // no output processing, force 8 bit input
    //
    config.c_cflag &= ~(CSIZE | PARENB);
    config.c_cflag |= CS8;
    //
    // One input byte is enough to return from read()
    // Inter-character timer off
    //
    config.c_cc[VMIN]  = 1;
    config.c_cc[VTIME] = 0;
    //
    // Communication speed (simple version, using the predefined
    // constants)
    //
    if(cfsetispeed(&config, B9600) < 0 || cfsetospeed(&config, B9600) < 0)
    {
        // ... error handling ...
    }
    //
    // Finally, apply the configuration
    //
    if(tcsetattr(fd, TCSAFLUSH, &config) < 0)
    {
        // ... error handling ...
    }

return fd;

}

int serial::serial_write(char* character)
{
    int bytes_written;
    bytes_written = write(my_serial_fd, character, sizeof character);

    return bytes_written;
}

char* serial::serial_read()
{


}

int serial::serial_close(int fd)
{
    close(fd);
}
