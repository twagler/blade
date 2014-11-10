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

    float difference=0;
    float derivative=0;

    bool first = true;

    GPS way, way2;

    while(Autonomous)
    {
        unique_lock<mutex> lk_gps(gps_lock);
        cv_gps.wait(lk_gps);

        way.setLatitude(LATwaypoint[wayindex+1]);
        way.setLongitude(LONwaypoint[wayindex+1]);
        distance = gps_distance(way, gps);

        if (distance < ARRIVED || first)  //arrived @ waypoint
        {
            CalcWaypoint();
            wayindex++;

            //these can be gotten rid of afer we find a better way to
            //calculate path error
            DeltaXgoal = LATwaypoint[wayindex+1] - LATwaypoint[wayindex];
            DeltaYgoal = LONwaypoint[wayindex+1] - LONwaypoint[wayindex];

            way.setLatitude(LATwaypoint[wayindex]);
            way.setLongitude(LONwaypoint[wayindex]);
            way2.setLatitude(LATwaypoint[wayindex+1]);
            way2.setLongitude(LONwaypoint[wayindex+1]);

            PathLength = gps_distance(way,way2);
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

        //DEBUG
        printf("\r\n");
        printf("*********************************************************\r\n");
        printf("*            Error Correction Algorithm I/O             *\r\n");
        printf("*-------------------------------------------------------*\r\n");
        printf("*| Current waypoint:\t%.4i\t\t\t       |*\r\n",
               wayindex);
        printf("*| Current Location:\t%f N,\t%f E   |*\r\n",
               gps.getLatitude(), gps.getLongitude());
        printf("*| Target Location:\t%f N,\t%f E   |*\r\n",
               LATwaypoint[wayindex+1], LONwaypoint[wayindex+1]);
        printf("*| Distance to Target:\t%011.0f centimeters\t       |*\r\n",
               distance);
        printf("*-------------------------------------------------------*\r\n");
        printf("* Path Length: %f centimeters\t\t\t*\r\n", PathLength);
        printf("* Path Error:\t%f centimeters\t\t\t*\r\n", difference);
        printf("* Gains:\tP: %f I: %f D: %f\t*\r\n",Kp,Ki,Kd);
        printf("* Corrections:\tP: %f I: %f D: %f\t*\r\n",
               difference, integral, derivative);
        printf("* Total Correction:\t%3d\t\t\t\t*\r\n", adjustment);
        //DEBUG
    }
    return;
}


