#ifndef MOWER_H
#define MOWER_H

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
#define MIN_SPEED -5
#define MAX_SPEED  55

#define AUTONOMOUS  0
#define MANUAL      1

#define GPS_SERVER "greenwellsoftware.com"
#define GPS_PORT "50001"
#define CONTROL_PORT "13333"   // port we're listening on
#define MAXDATASIZE 32768

class Mower
{
public:
    Mower();
private:

    thread thread_control_server;
    thread thread_GPS_receiver;
    thread thread_speed_setting;
    thread thread_navigation;

    GPS myCurrentLocation;
    ControlServer my_control_server;
    NMEAParser my_NMEA_parser;
    MotorDriver myMotors;

    mutex my_gps_lock;
    condition_variable my_cv_gps;

    mutex my_drive_lock;
    condition_variable my_cv_drive;

    signed char my_adjustment;
    signed char my_target_speed;
};

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
int ControlServerInit(const char*);
void *get_in_addr(struct sockaddr *sa);
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

extern signed char adjustment;
extern signed char targetspeed;

extern bool Autonomous;

extern NMEAParser parser;

extern ControlServer cs;

#endif


