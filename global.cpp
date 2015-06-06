#include "global.h"

Global::Global()
{
    myTargetSpeed = 35;

    myKp = 0.10;
    myKi = 0.00;
    myKd = 0.01;
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

float Global::get_myKp()
{
    return myKp;
}

int Global::set_myKp(float kp)
{
    myKp = kp;
    return 0;
}

float Global::get_myKi()
{
    return myKi;
}

int Global::set_myKi(float ki)
{
    myKi = ki;
    return 0;
}

float Global::get_myKd()
{
    return myKd;
}

int Global::set_myKd(float kd)
{
    myKd = kd;
    return 0;
}
