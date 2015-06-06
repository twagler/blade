#ifndef GLOBAL_H
#define GLOBAL_H

#include "gps.h"

class Global
{
public:
    Global();

    GPS get_myCurrentLocation();
    int set_myCurrentLocation(GPS);

    GPS get_myPreviousWaypoint();

    GPS get_myNextWaypoint();

    signed char get_myAdjustment();
    int set_myAdjustment(signed char);

    signed char get_myTargetSpeed();
    int set_myTargetSpeed(signed char);

    float get_myKp();
    int set_myKp(float);

    float get_myKi();
    int set_myKi(float);

    float get_myKd();
    int set_myKd(float);

private:
    GPS myCurrentLocation;
    GPS myPreviousWaypoint;
    GPS myNextWaypoint;

    signed char myAdjustment;
    signed char myTargetSpeed;

    float myKp;
    float myKi;
    float myKd;

};

#endif // GLOBAL_H
