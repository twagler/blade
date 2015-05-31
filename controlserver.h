#ifndef CONTROLSERVER_H
#define CONTROLSERVER_H
#include "gps.h"

class ControlServer
{

public:

    ControlServer();
    void run();
    void ParseCommand(char[], int);
    int UpdateInfo();
    int set_previous_location(GPS);
    int set_current_location(GPS);
    int set_next_location(GPS);

private:
    int initialize();

    //information to relay
    GPS my_previous_location, my_current_location, my_next_location;
    //GPS[8] waypoints;  //this will be replaced with lawnmap later
    signed char left_motor, right_motor;


    enum commands
    {
        MODE,
            MODE_AUTO,
            MODE_MANUAL,
        START,
            START_1,
            START_2,
        STOP,
        GET,
            CURR_LOC,
            PREV_LOC,
            NEXT_LOC
    };

};

#endif
