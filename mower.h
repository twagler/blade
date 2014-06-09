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
#include "gps.h"
#include "motordriver.h"
#include "joystick.h"
#include "controlserver.h"
#include "ultrasonic.h"
#include "libSerial/SerialStream.h"

#define NUM_ULTRAS 5
#define MIN_SPEED -126
#define MAX_SPEED  126

#define AUTONOMOUS  0
#define MANUAL      1

void UpdateUltraData(void);
void WaypointNavigation(void);
void CalcWaypoint(void);
void Print(GPS);
void SetSpeeds(void);
void ReadGPS(void);
void WriteGPS(void);
void JoystickTest(void);
void ControlSwitcher(void);
void InitTCPServer(void);
void InitUltraServer(void);
void openSerial(void);

extern int uSonic[];

extern float LATwaypoint[];
extern float LONwaypoint[];

extern mutex gps_lock;
extern condition_variable cv_gps;
extern GPS gps;

extern mutex drive_lock;
extern condition_variable cv_drive;

extern MotorDriver motors;

extern char adjustment;
extern char targetspeed;

extern bool Autonomous;


