#include "gps.h"
#include <math.h>
#define d2r (M_PI / 180.0)

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

float GPS::distance(GPS one, GPS two)
{
    double dlong = (two.getLongitude() - one.getLongitude())*d2r;
    double dlat = (two.getLatitude() - one.getLatitude())*d2r;
    double a1 = sin(dlat/2.0);
    double a2 = sin(dlong/2.0);
    double a = (a1*a1) + cos(one.getLatitude()*d2r) * cos(two.getLatitude()*d2r) * (a2*a2);
    double c = 2 * atan2(sqrt(a), sqrt(1-a));
    double d = 2.54 * 12 * 5280 * 3956 * c; //cm, in, ft, scaler

    return d;
}
