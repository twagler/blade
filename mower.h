using namespace std;
#include <stdio.h>
#include <iostream>
#include "gps.h"
#include "motordriver.h"
#include <mutex>
#include <condition_variable>

#define NUM_ULTRAS 5

void UpdateUltraData(void);
void WaypointNavigation(void);
void CalcWaypoint(void);
void Print(GPS);
void ReadGPS();
void WriteGPS();

extern int uSonic[];

extern float LATwaypoint[];
extern float LONwaypoint[];

extern mutex gps_lock;
extern char adjustment;
extern bool ready;
extern GPS gps;
extern char adjustment;


