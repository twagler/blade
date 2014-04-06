#include "gps.h"

GPS::GPS(float lat,float lon,int tim)
{
    myLatitude = lat;
    myLongitude = lon;
    myTime = tim;
}

float GPS::getLatitude() const
{
    return myLatitude;
}
float GPS::getLongitude() const
{
    return myLongitude;
}
int GPS::getTime() const
{
    return myTime;
}
