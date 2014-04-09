#include "mower.h"
#include <thread>
using namespace std;

int main() {

    char targetspeed = 65;

    char trash, adjustment;
    float lat, lon;
    int time;
    MotorDriver motors();
    thread rangeSensors(UpdateUltraData);

    while (1){

//        rangeSensors.
        cin >> lat >> trash >> lon >> trash >> time;
        GPS gps(lat, lon, time);
        adjustment = WaypointNavigation(gps);
        //motors.SetSpeeds(adjustment, targetspeed);
        //rangeSensors.join();
        //Print(gps);
    }
}
