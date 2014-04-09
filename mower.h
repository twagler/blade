#include <stdio.h>
#include <iostream>
#include "gps.h"
#include "motordriver.h"


#define NUM_ULTRAS 5

void UpdateUltraData(void);
char WaypointNavigation(GPS);
void CalcWaypoint(void);
void Print(GPS);

extern int uSonic[];

extern float LATwaypoint[];
extern float LONwaypoint[];

