#ifndef GLOBAL_H
#define GLOBAL_H

#include "gps.h"

class Global
{
public:
    Global();

    GPS get_myCurrentLocation();
    int set_myCurrentLocation(GPS);

private:
    GPS myCurrentLocation;
};

#endif // GLOBAL_H
