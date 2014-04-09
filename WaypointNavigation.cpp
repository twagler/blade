#include "mower.h"
#include <math.h>

#define Kp        0.10
#define Kd        0.10
#define Ki        0.10
#define ARRIVED   .00005

char WaypointNavigation(GPS gps) {

    static float distance   = 0;
    static float DeltaXgoal = 0;
    static float DeltaYgoal = 0;
    static float PathLength = 0;
    static float lasterror  = 0;
    static float integral   = 0;

    static int wayindex = 0;

    char difference=0;
    char derivative=0;
    char adjustment=0;

    float DXpath;
    float DYpath;

    //DEBUG
    std::cout << "Current wayindex: " << wayindex << "\r\n";
    std::cout << "Currently at: " << gps.getLatitude() << ", " << gps.getLongitude() << "\r\n";
    std::cout << "Traveling to: " << LATwaypoint[wayindex+1] << ", " << LONwaypoint[wayindex+1] << "\r\n";
    //DEBUG

    DXpath = LATwaypoint[wayindex+1] - gps.getLatitude();  //not correct
    DYpath = LONwaypoint[wayindex+1] - gps.getLongitude(); //not correct

    distance = sqrt((DXpath*DXpath) + (DYpath*DYpath));    //earth is flat?
    std::cout << "Distance to target: " << distance << "\r\n";

    if (distance < ARRIVED)  //arrived @ waypoint
    {
        CalcWaypoint();
        wayindex++;
        DeltaXgoal = LATwaypoint[wayindex+1] - LATwaypoint[wayindex];
        DeltaYgoal = LONwaypoint[wayindex+1] - LONwaypoint[wayindex];

        PathLength = sqrt((DeltaXgoal*DeltaXgoal) + (DeltaYgoal*DeltaYgoal));
    }

    else { //Travel toward waypoint

        difference = ((DeltaXgoal * (LATwaypoint[wayindex] - gps.getLongitude()))
                      -(DeltaYgoal * (LONwaypoint[wayindex] - gps.getLatitude()))) / PathLength;

        derivative = difference - lasterror;
        integral = integral + (difference+lasterror)/2;
        lasterror = difference;
        
        adjustment = Kp*difference+Kd*derivative+Ki*integral;    //PID
    }
    return adjustment;
}


