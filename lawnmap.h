#include <vector>
#include <time.h>
#ifndef LAWNMAP_H
#define LAWNMAP_H

struct COORDINATE {
    bool mowed;
    bool object;
    double currentGPSx;
    double currentGPSy;
    double fromGPSx;
    double fromGPSy;
    double toGPSx;
    double toGPSy;
    time_t mowedTime;
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

};

#endif // LAWNMAP_H
