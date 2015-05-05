#include "mower.h"

#define Kp        0.1000
#define Ki        0.0000
#define Kd        0.0005
#define ARRIVED   0.00005

#define MAX_ADJ     50
#define MIN_ADJ     -50

void WaypointNavigation() {

    //exp
//    float deltaLAT, deltaLON, deltaDIST;

    //exp

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

    float side_a, side_b, side_c, brng_a, brng_b, angle_A, dLon, y, x;

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

        if (distance < ARRIVED || first)  //arrived @ waypoint
        {
            //CalcWaypoint();
            wayindex++;

            way.setLatitude(LATwaypoint[wayindex]);
            way.setLongitude(LONwaypoint[wayindex]);
            way2.setLatitude(LATwaypoint[wayindex+1]);
            way2.setLongitude(LONwaypoint[wayindex+1]);

            PathLength = gps_distance(way,way2);
            first = false;

            //experiment
            dLon = way2.getLongitude_radians() - way.getLongitude_radians();
            y = sin(dLon) * cos(way2.getLatitude_radians());
            x = cos(way.getLatitude_radians())*sin(way2.getLatitude_radians())-
                    sin(way.getLatitude_radians())*cos(way2.getLatitude_radians())*cos(dLon);
            brng_a = atan2(y,x)*M_PI/180;

            //deltaLAT = way2.getLatitude() - way.getLatitude();
            //deltaLON = way2.getLongitude() - way.getLongitude();
            //deltaDIST = sqrt(deltaLAT*deltaLAT+deltaLON*deltaLON);
            //exp

        }

        else //Travel toward waypoint
        {

            //calculate difference using...
            //you thought you'd never use this again...
            //the law of cosines!!!

            // a^2 = b^2 + c^2 -2bc*cos(A)

            //let's solve for angle A first

            side_a = gps_distance(way, way2);
            side_b = gps_distance(gps, way);
            side_c = gps_distance(gps, way2);

            //2bc*cos(A) = b^2 + c^2 - a^2
            //cos(A) = (b^2 + c^2 - a^2)/2bc
            //cos^(-1)((cos(A)) = cos^(-1)((b^2 + c^2 - a^2)/2bc)
            //A = cos^(-1)((b^2 + c^2 - a^2)/2bc)

            angle_A = acos((side_b*side_b+side_c*side_c-side_a*side_a)/(2*side_b*side_c));

            //now that we have angle A, our unsigned error should be
            //side_b*sin(angle_A)

            difference = side_b*sin(angle_A);

            //now we have to decide which side of the path we're on
            //in order to sign the error

            dLon = gps.getLongitude_radians() - way.getLongitude_radians();
            y = sin(dLon) * cos(gps.getLatitude_radians());
            x = cos(way.getLatitude_radians())*sin(gps.getLatitude_radians())-
                    sin(way.getLatitude_radians())*cos(gps.getLatitude_radians())*cos(dLon);
            brng_b = atan2(y,x)*M_PI/180;

            if(brng_b < brng_a)
                difference = difference*-1;


            printf("\r\n");
            printf("*********************************************************\r\n");
            printf("*           Triangle Stuff...                           *\r\n");
            printf("*-------------------------------------------------------*\r\n");
            printf("Side A: %f\r\n", side_a);
            printf("Side B: %f\r\n", side_b);
            printf("Side C: %f\r\n", side_c);
            printf("Angle A (degrees): %f\r\n", angle_A*180/M_PI);
            printf("Side B * sin(Angle_A): %f\r\n", difference);
            printf("*********************************************************\r\n");
            printf("Target Bearing:  %f\r\n", brng_a);
            printf("Current Bearing: %f\r\n", brng_b);
            printf("*********************************************************\r\n");

            //difference = (sqrt((4*sideA*sideA*sideB*sideB)-pow((sideA*sideA+sideB*sideB-sideC*sideC),2)))/(2*sideA);

            //exp
            //difference = (deltaLON*(way.getLatitude()-gps.getLatitude()) - deltaLAT*(way.getLongitude()-gps.getLongitude())/deltaDIST);
            //exp

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


