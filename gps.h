#ifndef GPS_H
#define GPS_H

#include <time.h>
#include <math.h>
#include <string>
#include <sstream>
#include <iomanip>

class GPS
{
public:
    GPS();
    GPS(double, double);

    double getLatitude(void) const;
    double getLongitude(void) const;
    double getLatitude_radians(void) const;
    double getLongitude_radians(void) const;
    double getAltitude(void) const;
    unsigned long getSentenceCount(void) const;
    unsigned int getSignalQuality(void) const;
    unsigned int getSatelitesInUse(void) const;
    tm getTime(void) const;

    void setLatitude(double);
    void setLongitude(double);
    void setAltitude(double);
    void setSentenceCount(unsigned long);
    void setSignalQuality(unsigned int);
    void setSatelitesInUse(unsigned int);

    void setTime(tm);

    std::string toString();

private:

    double myLatitude;
    double myLongitude;
    double myAltitude;
    unsigned long mySentenceCount;
    unsigned int mySignalQuality;
    unsigned int mySatelitesInUse;
    tm myTime;
};

#endif // GPS_H
