// GPSInfo.h: interface for the GPSInfo class.
//
//////////////////////////////////////////////////////////////////////

#include <time.h>

struct GPSInfo  
{
public:
    double m_latitude;
    double m_longitude;
    double m_altitude;
    unsigned long m_nSentences;
    unsigned int m_signalQuality;
    unsigned int m_satelitesInUse;
    tm m_time;

    GPSInfo();
    virtual ~GPSInfo();
};
