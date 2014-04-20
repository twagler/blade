#include "mower.h"
#include <math.h>

#define Kp        0.10
#define Kd        0.10
#define Ki        0.10
#define ARRIVED   .00005

void WaypointNavigation() {

    cout << "Starting Waypoint Navigation thread...\r\n";

    float distance   = 0;
    float DeltaXgoal = 0;
    float DeltaYgoal = 0;
    float PathLength = 0;
    float lasterror  = 0;
    float integral   = 0;

    int wayindex = 0;

    char difference=0;
    char derivative=0;

    float DXpath;
    float DYpath;

    bool first = true;

    while(1)
    {
        unique_lock<mutex> lk_gps(gps_lock);
        cv_gps.wait(lk_gps);

        //DEBUG
        cout << "Received: " << gps.getLatitude() << "," << gps.getLongitude() << "," << gps.getTime() << "\r\n";
        cout << "Current wayindex: " << wayindex << "\r\n";
        cout << "Currently at: " << gps.getLatitude() << ", " << gps.getLongitude() << "\r\n";
        cout << "Traveling to: " << LATwaypoint[wayindex+1] << ", " << LONwaypoint[wayindex+1] << "\r\n";
        //DEBUG

        DXpath = LATwaypoint[wayindex+1] - gps.getLatitude();  //not correct
        DYpath = LONwaypoint[wayindex+1] - gps.getLongitude(); //not correct

        distance = sqrt((DXpath*DXpath) + (DYpath*DYpath));    //earth is flat? (close enough)
        cout << "Distance to target: " << distance << "\r\n";

        if (distance < ARRIVED || first)  //arrived @ waypoint
        {
            CalcWaypoint();
            wayindex++;
            DeltaXgoal = LATwaypoint[wayindex+1] - LATwaypoint[wayindex];
            DeltaYgoal = LONwaypoint[wayindex+1] - LONwaypoint[wayindex];

            PathLength = sqrt((DeltaXgoal*DeltaXgoal) + (DeltaYgoal*DeltaYgoal));
            first = false;
        }

        else //Travel toward waypoint
        {

            difference = ((DeltaXgoal * (LATwaypoint[wayindex] - gps.getLongitude()))
                          -(DeltaYgoal * (LONwaypoint[wayindex] - gps.getLatitude()))) / PathLength;

            derivative = difference - lasterror;
            integral = integral + (difference+lasterror)/2;
            lasterror = difference;

            drive_lock.lock();

            adjustment = Kp*difference+Kd*derivative+Ki*integral;    //PID

            drive_lock.unlock();
            cv_drive.notify_one();
        }
        lk_gps.unlock();
    }
}


