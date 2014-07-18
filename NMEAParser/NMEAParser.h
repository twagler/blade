// NMEAParser.h: interface for the NMEAParser class.
//
//////////////////////////////////////////////////////////////////////

#define GPS_SERIAL_PORT

#include "gps.h"
#include "serialport.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>    // needed for serial port
#include <ctype.h>      // needed for isalpha(), etc...
#include <string.h>     // needed for memsetc++

#define SERIAL_DEVICE "/dev/ttyUSB0"

class NMEAParser  
{
public:
	NMEAParser();
	virtual ~NMEAParser();
    void Parse(const char *buf, const unsigned int bufSize);
    GPS& GetActualGPSInfo();
    void read_serial();

private:

    void ParseRecursive(const char ch);
    void ParseNMEASentence(const char *addressField, const char *buf, const unsigned int bufSize);
    void ProcessGPGGA(const char *buf, const unsigned int bufSize);
    void ProcessGPGSA(const char *buf, const unsigned int bufSize);
    void ProcessGPGSV(const char *buf, const unsigned int bufSize);
    void ProcessGPRMB(const char *buf, const unsigned int bufSize);
    void ProcessGPRMC(const char *buf, const unsigned int bufSize);
    void ProcessGPZDA(const char *buf, const unsigned int bufSize);

    GPS myGPSInfo;
    //SerialPort mySerialPort;

};