using namespace std;

#include "mower.h"

int main() {

    int test;
    //thread RangeSensors(UpdateUltraData);
    //thread ReceiveGPS(ReadGPS);
    //thread AdjustSpeed(SetSpeeds);
    //thread ControlSupervisor(ControlSwitcher);
    //thread ReadCommands(ControlCommands);

    ControlServer cs;

    test = cs.run();

    while(1)  //bad bad bad bad bad
       this_thread::sleep_for(chrono::milliseconds(500));
}

