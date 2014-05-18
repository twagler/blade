using namespace std;

#include "mower.h"

int main() {

    //thread RangeSensors(UpdateUltraData);
    //thread ReceiveGPS(ReadGPS);
    //thread Navigate(WaypointNavigation);
    //thread AdjustSpeed(SetSpeeds);
    thread PS3(JoystickTest);
    //thread ReadCommands(ControlCommands);

    while(1);
}

