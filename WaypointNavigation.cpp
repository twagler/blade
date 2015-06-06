#include "mower.h"
#include "global.h"

/*
 *      LATITUDE is North/South and unit equal
 *      LONGITUDE is East/West and not unit equal
 *
 * */

#define Kp        0.5
#define Ki        0.0000
#define Kd        0.05
#define ARRIVED   35 //cm

#define MAX_ADJ     35
#define MIN_ADJ     -50

void WaypointNavigation() {

    float deltaLAT, deltaLON, temp1, temp2, temp3, temp4;

    cout << "Starting Waypoint Navigation thread...\r\n";

    float distance   = 0;
    float PathLength = 0;
    float lasterror  = 0;
    float integral   = 0;

    int wayindex = 0;

    float difference=0;
    float derivative=0;
    signed int temp_adj;

    GPS way, way2;

    unique_lock<mutex> lk_gps(gps_lock);
    cv_gps.wait(lk_gps);

    //set first waypoint to where we are

    LATwaypoint[0] = myGlobal.get_myCurrentLocation().getLatitude();
    LONwaypoint[0] = myGlobal.get_myCurrentLocation().getLongitude();
    LATwaypoint[1] = myGlobal.get_myCurrentLocation().getLatitude();
    LONwaypoint[1] = myGlobal.get_myCurrentLocation().getLongitude();

    lk_gps.unlock();

    wayindex++;
    way.setLatitude(LATwaypoint[wayindex]);
    way.setLongitude(LONwaypoint[wayindex]);
    way2.setLatitude(LATwaypoint[wayindex+1]);
    way2.setLongitude(LONwaypoint[wayindex+1]);

    deltaLAT = gps_distance(GPS(way2.getLatitude(), way2.getLongitude()),GPS(way.getLatitude(), way2.getLongitude()));
    if(way2.getLatitude()-way.getLatitude() < 0)
        deltaLAT*=-1;
    deltaLON = gps_distance(GPS(way.getLatitude(), way.getLongitude()),GPS(way.getLatitude(), way2.getLongitude()));
    if((way2.getLongitude()-way.getLongitude()) < 0)
        deltaLON*=-1;
    PathLength = gps_distance(way,way2);

    //I believe we need to figure out what sign belongs where according to our heading
    //for instance when traveling northisheastish, the deltaLAT should be +
    //and the deltaLON should be (something)


    while(true)
    {

        unique_lock<mutex> lk_gps(gps_lock);
        cv_gps.wait(lk_gps);

        distance = gps_distance(way2,myGlobal.get_myCurrentLocation());

        if (distance < ARRIVED)  //arrived @ waypoint
        {
            wayindex++;

            way.setLatitude(LATwaypoint[wayindex]);
            way.setLongitude(LONwaypoint[wayindex]);
            way2.setLatitude(LATwaypoint[wayindex+1]);
            way2.setLongitude(LONwaypoint[wayindex+1]);

            deltaLAT = gps_distance(GPS(way2.getLatitude(), way2.getLongitude()),GPS(way.getLatitude(), way2.getLongitude()));
            if(way2.getLatitude()-way.getLatitude() < 0)
                deltaLAT*=-1;
            deltaLON = gps_distance(GPS(way.getLatitude(), way.getLongitude()),GPS(way.getLatitude(), way2.getLongitude()));
            if((way2.getLongitude()-way.getLongitude()) < 0)
                deltaLON*=-1;

            PathLength = gps_distance(way,way2);

        }

        else //Travel toward waypoint
        {
            //magnitude
            temp1 = gps_distance(GPS(myGlobal.get_myCurrentLocation().getLatitude(),myGlobal.get_myCurrentLocation().getLongitude()),
                                 GPS(way.getLatitude(),myGlobal.get_myCurrentLocation().getLongitude()));

            //sign
            temp2 = way.getLatitude() - myGlobal.get_myCurrentLocation().getLatitude();

            //magnitude
            temp3 = gps_distance(GPS(way.getLatitude(),way.getLongitude()),
                                 GPS(myGlobal.get_myCurrentLocation().getLatitude(),way.getLongitude()));

            //sign
            temp4 = way.getLongitude() - myGlobal.get_myCurrentLocation().getLongitude();

            //sign the magnitude
            if(temp2 < 0)
                temp1*=-1;

            if(temp4 < 0)
                temp3*=-1;

            difference = ((deltaLON*temp1)-(deltaLAT*temp3))/PathLength;

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

            //adjustment = temp_adj;  //assuming this auto-casts?
            myGlobal.set_myAdjustment(temp_adj);


            drive_lock.unlock();
            cv_drive.notify_one();
            //DEBUG
            printf("\r\n");
            printf("*********************************************************\r\n");
            printf("*           Blade Navigation Inputs/Outputs             *\r\n");
            printf("*-------------------------------------------------------*\r\n");
            printf("*| Current waypoint:\t%.4i\tMode: %d\t\t       |*\r\n",
                   wayindex,Autonomous);
            printf("*| Solution Quality:\t%i   Number of Satellites: %.2i   |*\r\n",
                   myGlobal.get_myCurrentLocation().getSignalQuality(),
                   myGlobal.get_myCurrentLocation().getSatelitesInUse());
            printf("*| Current Location:\t%f N,\t%f W   |*\r\n",
                   myGlobal.get_myCurrentLocation().getLatitude(),
                   myGlobal.get_myCurrentLocation().getLongitude());
            printf("*| Target Location:\t%f N,\t%f W   |*\r\n",
                   LATwaypoint[wayindex+1], LONwaypoint[wayindex+1]);
            printf("*| Distance to Target:\t%011.0f centimeters\t       |*\r\n",
                   distance);
            printf("*-------------------------------------------------------*\r\n");
            printf("* Path Length: %f centimeters\t\t\t*\r\n", PathLength);
            printf("* Path Error:\t%f centimeters\t\t\t*\r\n", difference);
            printf("* Gains:\tP: %f I: %f D: %f\t*\r\n",Kp,Ki,Kd);
            printf("* Corrections:\tP: %f I: %f D: %f\t*\r\n",
                   difference, integral, derivative);
            printf("* Total Correction:\t%3d\t\t\t\t*\r\n", myGlobal.get_myAdjustment());
            //DEBUG

        }
        lk_gps.unlock();
    }
    return;
}


