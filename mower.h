using namespace std;
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <string.h>
#include <math.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "gps.h"
#include "motordriver.h"
#include "joystick.h"
#include "controlserver.h"
#include "ultrasonic.h"
#include "libGPIO/libgpio.h"
#include "NMEAParser/NMEAParser.h"

#define DEBUG

#define d2r (M_PI / 180.0)
#define NUM_ULTRAS 5
#define MIN_SPEED -126
#define MAX_SPEED  126

#define AUTONOMOUS  0
#define MANUAL      1

#define GPS_SERVER "tegra-ubuntu"
#define GPS_PORT "1334"
#define MAXDATASIZE 32768

void UpdateUltraData(void);
void WaypointNavigation(void);
void CalcWaypoint(void);
void Print(GPS);
void SetSpeeds(void);
void ReadGPS_RTKLIB(void);
void ReadGPS_NMEA(void);
void WriteGPS(void);
void JoystickTest(void);
void ControlSwitcher(void);
void InitTCPServer(void);
void InitUltraServer(void);
void InitGPSThread(void);
void openSerial(void);
float gps_distance(GPS,GPS);


extern int uSonic[];

extern double LATwaypoint[];
extern double LONwaypoint[];

extern mutex gps_lock;
extern condition_variable cv_gps;
extern GPS gps;

extern mutex drive_lock;
extern condition_variable cv_drive;

extern MotorDriver motors;

extern char adjustment;
extern char targetspeed;

extern bool Autonomous;

extern NMEAParser parser;


