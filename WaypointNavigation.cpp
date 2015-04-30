#include "mower.h"

#define Kp        0.1000
#define Ki        0.0000
#define Kd        0.0005
#define ARRIVED   0.00005

void WaypointNavigation() {

    cout << "Starting Waypoint Navigation thread...\r\n";

    float distance   = 0;
    float PathLength = 0;
    float lasterror  = 0;
    float integral   = 0;

    int wayindex = 0;

    float difference=0;
    float derivative=0;

    bool first = true;

    GPS way, way2;

    while(true)
    {

        unique_lock<mutex> lk_gps(gps_lock);
        cv_gps.wait(lk_gps);

        //set first waypoint to where we are
        LATwaypoint[0] = gps.getLatitude();
        LONwaypoint[0] = gps.getLongitude();

        way.setLatitude(LATwaypoint[wayindex]);
        way.setLongitude(LONwaypoint[wayindex]);
        way2.setLatitude(LATwaypoint[wayindex+1]);
        way2.setLongitude(LONwaypoint[wayindex+1]);
        distance = gps_distance(way2, gps);

        if (distance < ARRIVED || first)  //arrived @ waypoint
        {
            //CalcWaypoint();
            wayindex++;

            way.setLatitude(LATwaypoint[wayindex+1]);
            way.setLongitude(LONwaypoint[wayindex+1]);
            way2.setLatitude(LATwaypoint[wayindex]);
            way2.setLongitude(LONwaypoint[wayindex]);

            PathLength = gps_distance(way,way2);
            first = false;
        }

        else //Travel toward waypoint
        {

            //calculate difference using Heron's Formula
            //combined with a little algebra

            //A = (1/2)b*h
            //A = sqrt(s(s-a)(s-b)(s-c)) where s is semiperimeter

            //so

            float sideA, sideB, sideC;

            sideA = gps_distance(way, way2);
            sideB = gps_distance(gps, way);
            sideC = gps_distance(gps, way2);

            difference = (sqrt((4*sideA*sideA*sideB*sideB)-
                               pow((sideA*sideA+sideB*sideB-sideC*sideC),2)))
                    /(2*sideA);

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
        printf("*           Blade Navigation Inputs/Outputs             *\r\n");
        printf("*-------------------------------------------------------*\r\n");
        printf("*| Current waypoint:\t%.4i\tMode: %d\t       |*\r\n",
               wayindex,Autonomous);
        printf("*| Solution Quality:\t%i   Number of Satellites: %.2i   |*\r\n",
               gps.getSignalQuality(), gps.getSatelitesInUse());
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

        this_thread::sleep_for(chrono::milliseconds(500));
    }
    return;
}


