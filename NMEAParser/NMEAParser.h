// NMEAParser.h: interface for the NMEAParser class.
//
//////////////////////////////////////////////////////////////////////

#define SERVER "tegra-ubuntu"
#define PORT "1334"
#define MAXDATASIZE 1023

#include "gps.h"
#include "serialport.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>      // needed for isalpha(), etc...
#include <string.h>     // needed for memsetc++
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

class NMEAParser  
{
 public:
  NMEAParser();
  virtual ~NMEAParser();
  void Parse(const char *buf, const unsigned int bufSize);
  GPS& GetActualGPSInfo();
  int read_RTKLIBserver();

 private:

  int TCPinit();
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
