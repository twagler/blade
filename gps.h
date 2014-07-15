#ifndef GPS_H
#define GPS_H

#include <time.h>

class GPS
{
public:
    GPS();
    GPS(double, double);

    double getLatitude(void) const;
    double getLongitude(void) const;
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
    void incrementSentenceCount(void);

    void setTime(tm);

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
