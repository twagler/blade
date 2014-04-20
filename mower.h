using namespace std;
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <string.h>
#include <math.h>
#include "gps.h"
#include "motordriver.h"

#define NUM_ULTRAS 5
#define MIN_SPEED -126
#define MAX_SPEED  126

void UpdateUltraData(void);
void WaypointNavigation(void);
void CalcWaypoint(void);
void Print(GPS);
void SetSpeeds();
void ReadGPS();
void WriteGPS();

extern int uSonic[];

extern float LATwaypoint[];
extern float LONwaypoint[];

extern mutex gps_lock;
extern condition_variable cv_gps;
extern GPS gps;

extern mutex drive_lock;
extern condition_variable cv_drive;

extern char adjustment;
extern char targetspeed;


