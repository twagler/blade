using namespace std;

#include "mower.h"

int main() {

    thread NetworkControl;
    //thread RangeSensors(UpdateUltraData);
    thread ReceiveGPS(ReadGPS);
    thread AdjustSpeed(SetSpeeds);
    thread ControlSupervisor(ControlSwitcher);

    NetworkControl = thread(InitTCPServer);

    while(1)  //bad bad bad bad bad
       this_thread::sleep_for(chrono::milliseconds(500));

    //if(ReceiveGPS.joinable())
    //    ReceiveGPS.join();;
    //if(AdjustSpeed.joinable())
    //    AdjustSpeed.join();
    //if(ControlSupervisor.joinable())
    //    ControlSupervisor.join();


    return(0);

}

