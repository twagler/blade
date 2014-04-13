#ifndef GPS_H
#define GPS_H

class GPS
{
public:
    GPS();
    GPS(float, float, int);
    float getLatitude() const;
    float getLongitude() const;
    int getTime() const;

    void setLatitude(float);
    void setLongitude(float);
    void setTime(int);

    float distance(GPS, GPS);

private:
    float myLatitude;
    float myLongitude;
    int myTime;
};

#endif // GPS_H
