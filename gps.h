#ifndef GPS_H
#define GPS_H

class GPS
{
public:
    GPS(float, float, int);
    float getLatitude() const;
    float getLongitude() const;
    int getTime() const;


private:
    float myLatitude;
    float myLongitude;
    int myTime;
};

#endif // GPS_H
