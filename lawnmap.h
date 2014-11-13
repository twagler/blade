using namespace std;
#include <vector>
#include <time.h>
#include "gps.h"
#include "mower.h"
#ifndef LAWNMAP_H
#define LAWNMAP_H

//close enough ~2cm
#define CLOSE_ENOUGH 2

struct LawnCoordinate
{
    double Latitude;
    double Longitude;
    bool object;
    time_t mowedTime;
    bool boundary;
};
struct BOUSTROPHEDON
{
    LawnCoordinate NE;
    LawnCoordinate NW;
    LawnCoordinate SE;
    LawnCoordinate SW;
    int size;
};

class LawnMap
{
public:
    LawnMap();
    LawnMap(vector<LawnCoordinate>);
    int boundaryCheck(LawnCoordinate);
    int atWaypoint(LawnCoordinate);
    LawnCoordinate atCoordinate(GPS);
    int mowedLawnCoordinate(LawnCoordinate);
    int canTurn();
    void nextRow();
    //bool lineGood(LawnCoordinate, LawnCoordinate);
    bool lineGood(int, int, int, int);

    vector<LawnCoordinate[4]> boustrophedons; //vector containing known boustrophedons.
    //Each boustrophendon has 4 corners, each with an lat and lon component
    vector< vector<LawnCoordinate> > map;
    int LatSize;
    int LonSize;
    void setCurrentGPS(double x, double y);
    void setFromGPS(double x, double y);
    void setToGPS(double x, double y);
    double *getCurrentGPS();
    double *getToGPS();
    double *getFromGPS();

private:
    LawnCoordinate currentLocation;
    LawnCoordinate fromLocation;
    LawnCoordinate toLocation;
};

#endif // LAWNMAP_H
