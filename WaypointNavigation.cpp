#include "mower.h"
#include <math.h>

#define Kp        0.10
#define Kd        0.10
#define Ki        0.10
#define MIN_SPEED -127
#define MAX_SPEED 127
#define ARRIVED   .00005

void WaypointNavigation(GPS gps) {

    static float distance   = 0;
    static float DeltaXgoal = 0;
    static float DeltaYgoal = 0;
    static float PathLength = 0;
    static float lasterror  = 0;
    static float integral   = 0;

    static int wayindex = 0;

    float difference=0;
    float derivative=0;
    float adjustment=0;

    float DXpath;
    float DYpath;

    //DEBUG
    std::cout << "Current wayindex: " << wayindex << "\r\n";
    std::cout << "Currently at: " << gps.getLatitude() << ", " << gps.getLongitude() << "\r\n";
    std::cout << "Traveling to: " << LATwaypoint[wayindex+1] << ", " << LONwaypoint[wayindex+1] << "\r\n";
    //DEBUG

    DXpath = LATwaypoint[wayindex+1] - gps.getLatitude();
    DYpath = LONwaypoint[wayindex+1] - gps.getLongitude();

    distance = sqrt((DXpath*DXpath) + (DYpath*DYpath));
    std::cout << "Distance to target: " << distance << "\r\n";

    if (distance < ARRIVED)  //arrived @ waypoint
    {
        CalcWaypoint();
        wayindex++;
        wayindex = wayindex%8;             //memory saver for 8bit MCU. we can probably do away with this

        DeltaXgoal = LATwaypoint[wayindex+1] - LATwaypoint[wayindex];
        DeltaYgoal = LONwaypoint[wayindex+1] - LONwaypoint[wayindex];

        PathLength = sqrt((DeltaXgoal*DeltaXgoal) + (DeltaYgoal*DeltaYgoal));
    }

    else { //Travel toward waypoint

        difference = ((DeltaXgoal * (LATwaypoint[wayindex] - gps.getLongitude()))
                      -(DeltaYgoal * (LONwaypoint[wayindex] - gps.getLatitude()))) / PathLength;
        derivative = difference - lasterror;
        integral = integral + (difference+lasterror)/2;
        
        adjustment = Kp*difference+Kd*derivative+Ki*integral;    //PID

        lasterror = difference;
        leftspeed = leftspeed + adjustment;
        rightspeed = rightspeed + adjustment;
       
       
        if (leftspeed < MIN_SPEED)                 //cap min motor speed
            leftspeed = MIN_SPEED;
        else if (leftspeed>MAX_SPEED)
            leftspeed = MAX_SPEED;                  //cap max motor speed

        if (rightspeed<MIN_SPEED)
            rightspeed = MIN_SPEED;
        else if (rightspeed>MAX_SPEED)
            rightspeed = MAX_SPEED;

    }
}


