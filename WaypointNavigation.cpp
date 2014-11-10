using namespace std;

#include "mower.h"

#define Kp        0.100
#define Kd        0.005
#define Ki        0.001
#define ARRIVED   0.00005

void WaypointNavigation() {

    cout << "Starting Waypoint Navigation thread...\r\n";

    float distance   = 0;
    float DeltaXgoal = 0;
    float DeltaYgoal = 0;
    float PathLength = 0;
    float lasterror  = 0;
    float integral   = 0;

    int wayindex = 0;

    int difference=0;
    int derivative=0;

    bool first = true;

    GPS way, way2;

    while(Autonomous)
    {
        unique_lock<mutex> lk_gps(gps_lock);
        cv_gps.wait(lk_gps);

        //DEBUG
        cout << "Current wayindex: " << wayindex << "\r\n";
        cout << "Currently at: " << gps.getLatitude() << ", " <<
                gps.getLongitude() << "\r\n";
        cout << "Traveling to: " << LATwaypoint[wayindex+1] << ", " <<
                LONwaypoint[wayindex+1] << "\r\n";
        //DEBUG

        way.setLatitude(LATwaypoint[wayindex+1]);
        way.setLongitude(LONwaypoint[wayindex+1]);

        distance = gps_distance(way, gps);

        cout << "Distance to target: " << distance << "\r\n";

        if (distance < ARRIVED || first)  //arrived @ waypoint
        {
            CalcWaypoint();
            wayindex++;

            //these can be gotten rid of afer we find a better way to
            //calculate path error
            DeltaXgoal = LATwaypoint[wayindex+1] - LATwaypoint[wayindex];
            DeltaYgoal = LONwaypoint[wayindex+1] - LONwaypoint[wayindex];

            way2.setLatitude(LATwaypoint[wayindex+1]);
            way2.setLongitude(LONwaypoint[wayindex+1]);

            PathLength = gps_distance(gps,way2);

            printf("path length: %fcm\r\n",PathLength);
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
    return;
}


