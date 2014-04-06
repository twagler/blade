#include "mower.h"
#include <thread>
using namespace std;

int main() {

    char trash;
    float lat, lon;
    int time;

    while (1){
        thread rangeSensors(UpdateUltraData);
        cin >> lat >> trash >> lon >> trash >> time;
        GPS gps(lat, lon, time);
        WaypointNavigation(gps);
        rangeSensors.join();
        //Print(gps);
    }
}
