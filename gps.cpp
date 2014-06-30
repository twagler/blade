#include "gps.h"

GPS::GPS()
{
    myLatitude = 0;
    myLongitude = 0;
    myTime = 0;
}

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
void GPS::setLatitude(float lat)
{
    myLatitude = lat;
}
void GPS::setLongitude(float lon)
{
    myLongitude = lon;
}
void GPS::setTime(int time)
{
    myTime = time;
}
