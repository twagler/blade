#include "mower.h"

#define Kp        100
#define Ki        0
#define Kd        50
#define ARRIVED   .00005

#define MAX_ADJ     50
#define MIN_ADJ     -50

void WaypointNavigation() {

    float deltaLAT, deltaLON, deltaDIST;

    cout << "Starting Waypoint Navigation thread...\r\n";

    float distance   = 0;
    float PathLength = 0;
    float lasterror  = 0;
    float integral   = 0;

    int wayindex = 0;

    float difference=0;
    float derivative=0;
    signed int temp_adj;

    bool first = true;

    GPS way, way2;

    unique_lock<mutex> lk_gps(gps_lock);
    cv_gps.wait(lk_gps);
    //set first waypoint to where we are
    LATwaypoint[0] = gps.getLatitude();
    LONwaypoint[0] = gps.getLongitude();
    LATwaypoint[1] = gps.getLatitude();
    LONwaypoint[1] = gps.getLongitude();

    lk_gps.unlock();

    while(true)
    {

        unique_lock<mutex> lk_gps(gps_lock);
        cv_gps.wait(lk_gps);

        way.setLatitude(LATwaypoint[wayindex]);
        way.setLongitude(LONwaypoint[wayindex]);
        way2.setLatitude(LATwaypoint[wayindex+1]);
        way2.setLongitude(LONwaypoint[wayindex+1]);
        distance = gps_distance(way2, gps);

        cs.set_previous_location(way);
        cs.set_current_location(gps);
        cs.set_next_location(way2);

        if (distance < ARRIVED || first)  //arrived @ waypoint
        {
            wayindex++;

            way.setLatitude(LATwaypoint[wayindex]);
            way.setLongitude(LONwaypoint[wayindex]);
            way2.setLatitude(LATwaypoint[wayindex+1]);
            way2.setLongitude(LONwaypoint[wayindex+1]);

            first = false;

            deltaLAT = way2.getLatitude() - way.getLatitude();
            deltaLON = way2.getLongitude() - way.getLongitude();
            deltaDIST = sqrt(deltaLAT*deltaLAT+deltaLON*deltaLON);
            PathLength = gps_distance(way,way2);

        }

        else //Travel toward waypoint
        {

            difference = (deltaLON*(way.getLatitude()-gps.getLatitude()) - deltaLAT*(way.getLongitude()-gps.getLongitude())/deltaDIST);

            derivative = difference - lasterror;
            integral = integral + (difference+lasterror)/2;
            if(integral != integral) //catches NaN error from continually propagating
                integral = 0;
            lasterror = difference;

            //figure out some logical caps...
            temp_adj = Kp*difference+Kd*derivative+Ki*integral;    //PID
            if (temp_adj > MAX_ADJ)
                temp_adj = MAX_ADJ;
            else if (temp_adj < MIN_ADJ)
                temp_adj = MIN_ADJ;


            drive_lock.lock();

            adjustment = temp_adj;  //assuming this auto-casts?


            drive_lock.unlock();
            cv_drive.notify_one();

        }
        lk_gps.unlock();

        //DEBUG
        printf("\r\n");
        printf("*********************************************************\r\n");
        printf("*           Blade Navigation Inputs/Outputs             *\r\n");
        printf("*-------------------------------------------------------*\r\n");
        printf("*| Current waypoint:\t%.4i\tMode: %d\t\t       |*\r\n",
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
        printf("* Gains:\tP: %d\tI: %d\tD: %d\t\t\t*\r\n",Kp,Ki,Kd);
        printf("* Corrections:\tP: %f I: %f D: %f\t*\r\n",
               difference, integral, derivative);
        printf("* Total Correction:\t%3d\t\t\t\t*\r\n", adjustment);
        //DEBUG

        this_thread::sleep_for(chrono::milliseconds(500));
    }
    return;
}


