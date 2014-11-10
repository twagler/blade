using namespace std;

#include "mower.h"

int main() {

    //thread RangeSensors(InitUltraServer);
    thread ReceiveGPS(ReadGPS_NMEA);
    thread AdjustSpeed(SetSpeeds);
    thread ControlSupervisor(ControlSwitcher);
    //thread PS3(JoystickTest);
    thread NetworkControl(InitTCPServer);

    while(1)  //bad bad bad bad bad
        this_thread::sleep_for(chrono::milliseconds(500));
}

void InitTCPServer(void)
{
    printf("Starting Network Control Server...\r\n");
    ControlServer cs;
    cs.run();
    while(true);
}

void InitUltraServer(void)
{
    printf("Starting Ultrasonic Update Daemon...\r\n");
    UltraSonic us;
    while(true)
    {
        us.updateUltra();
        printf("Refreshed Sensors...\r\n");
        this_thread::sleep_for(chrono::seconds(1));
    }
}

void ControlSwitcher()
{
    //Initialize the thread object
    thread Navigate;

    //infinite loop here
    while(true)
    {
        //if we're operating in Autonomous mode, kick off the WaypointNavigation thread
        if(Autonomous)
            Navigate = thread(WaypointNavigation);

        //Continue to operate in autonomous mode until something kicks us out of auto
        while(Autonomous)
            this_thread::sleep_for(chrono::milliseconds(500));

        //join the thread when booted out of auto
        if(Navigate.joinable())
        {
            Navigate.join();
            printf("Killed Waypoint Navigation thread...\r\n");
        }

        //give the CPU a break...
        this_thread::sleep_for(chrono::milliseconds(500));
    }
}
