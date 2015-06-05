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

private:
    GPS myCurrentLocation;
    GPS myPreviousWaypoint;
    GPS myNextWaypoint;

    signed char myAdjustment;
    signed char myTargetSpeed;

};

#endif // GLOBAL_H
