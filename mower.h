#include <stdio.h>
#include <iostream>
#include "gps.h"


#define NUM_ULTRAS 5

void UpdateUltraData(void);
void WaypointNavigation(GPS);
void CalcWaypoint(void);
void Print(GPS);

extern int uSonic[];

extern int leftspeed;
extern int rightspeed;

extern float LATwaypoint[];
extern float LONwaypoint[];

