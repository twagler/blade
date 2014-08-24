#include <vector>
#include <time.h>
#ifndef LAWNMAP_H
#define LAWNMAP_H

struct COORDINATE {
    bool mowed;
    bool object;
    time_t mowedTime;
    bool boundary;
};
struct BOUSTROPHEDON {
    int xNE;
    int yNE;
    int xNW;
    int yNW;
    int xSE;
    int ySE;
    int xSW;
    int ySW;
    int size;
};

class LawnMap
{
public:
    LawnMap();
    LawnMap(std::vector<double[2]>);
    int boundaryCheck();
    int atWaypoint();
    int canTurn();
    void nextRow();

    std::vector<int[4][2]> boustrophedons; //vector containing known boustrophedons.  Each boustrophendon has 4 corners, each with an x and y component
    std::vector< std::vector<COORDINATE> > map;
    int xSize;
    int ySize;
    void setCurrentGPS(double x, double y);
    void setFromGPS(double x, double y);
    void setToGPS(double x, double y);
    double *getCurrentGPS();
    double *getToGPS();
    double *getFromGPS();

private:
    double currentGPSx;
    double currentGPSy;
    double fromGPSx;
    double fromGPSy;
    double toGPSx;
    double toGPSy;

};

#endif // LAWNMAP_H
