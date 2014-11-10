// NMEAParser.h: interface for the NMEAParser class.
//
//////////////////////////////////////////////////////////////////////

#include "gps.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>      // needed for isalpha(), etc...
#include <string.h>     // needed for memsetc++

class NMEAParser  
{
public:
    NMEAParser();
    virtual ~NMEAParser();
    GPS& Parse(const char *buf, const unsigned int bufSize);

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

};
