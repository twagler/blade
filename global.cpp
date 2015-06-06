#include "global.h"

Global::Global()
{
    myTargetSpeed = 35;
}

GPS Global::get_myCurrentLocation()
{
    return myCurrentLocation;
}

int Global::set_myCurrentLocation(GPS gps)
{
    myCurrentLocation = gps;
    return 0;
}

GPS Global::get_myPreviousWaypoint()
{
    return myPreviousWaypoint;
}

GPS Global::get_myNextWaypoint()
{
    return myNextWaypoint;
}

signed char Global::get_myAdjustment()
{
    return myAdjustment;
}

int Global::set_myAdjustment(signed char adj)
{
    myAdjustment = adj;
    return 0;
}

signed char Global::get_myTargetSpeed()
{
    return myTargetSpeed;
}

int Global::set_myTargetSpeed(signed char ts)
{
    myTargetSpeed = ts;
    return 0;
}
