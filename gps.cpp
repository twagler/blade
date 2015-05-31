#include "gps.h"

GPS::GPS()
{
    myLatitude = 0;
    myLongitude = 0;
    myAltitude = 0;
    mySignalQuality = 0;
    mySatelitesInUse = 0;
    //myTime = 0;
}

GPS::GPS(double lat,double lon)
{
    myLatitude = lat;
    myLongitude = lon;
    //myTime = tim;
}

std::string GPS::toString()
{
    std::string ret;
    std::stringstream ss;
    //ss << myLatitude << ",";
    //ss << myLongitude << ",";
    //ss << myAltitude << ",";
    //ss << mySatelitesInUse << "\r\n";
    ss << std::setprecision(12) << myLatitude << "," << myLongitude << ",";
    ss << myAltitude << "," << mySatelitesInUse << "\r\n";
    ret = ss.str();
    return ret;

    //char* buffer;
    //"GPS,LAT,LON,ALT,SATS
    //sprintf(buffer,"GPS,%f,%f,%f,%d",myLatitude,myLongitude,myAltitude,mySatelitesInUse);
    //return buffer;
}

double GPS::getLatitude() const
{
    return myLatitude;
}
double GPS::getLongitude() const
{
    return myLongitude;
}
double GPS::getLatitude_radians() const
{
    return myLatitude*180/M_PI;
}
double GPS::getLongitude_radians() const
{
    return myLongitude*180/M_PI;
}
double GPS::getAltitude() const
{
    return myAltitude;
}
unsigned int GPS::getSignalQuality() const
{
    return mySignalQuality;
}
unsigned int GPS::getSatelitesInUse() const
{
    return mySatelitesInUse;
}
tm GPS::getTime() const
{
    return myTime;
}
void GPS::setLatitude(double lat)
{
    myLatitude = lat;
}
void GPS::setLongitude(double lon)
{
    myLongitude = lon;
}
void GPS::setAltitude(double alt)
{
    myAltitude = alt;
}
void GPS::setSentenceCount(unsigned long sent_count)
{
    mySentenceCount = sent_count;
}
void GPS::setSignalQuality(unsigned int sig_qual)
{
    mySignalQuality = sig_qual;
}
void GPS::setSatelitesInUse(unsigned int sats)
{
    mySatelitesInUse = sats;
}
void GPS::setTime(tm time)
{
    myTime = time;
}
