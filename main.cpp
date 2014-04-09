#include "mower.h"
#include <thread>
using namespace std;

int main() {

    char trash, adjustment;
    float lat, lon;
    int time;
    MotorDriver motors();

    while (1){
        thread rangeSensors(UpdateUltraData);
        cin >> lat >> trash >> lon >> trash >> time;
        GPS gps(lat, lon, time);
        adjustment = WaypointNavigation(gps);
        motors.SetAdjustment(adjustment);
        rangeSensors.join();
        //Print(gps);
    }
}
