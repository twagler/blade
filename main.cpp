#include "mower.h"
#include "global.h"

int main() {

    //thread RangeSensors(InitUltraServer);
    thread ReceiveGPS(ReadGPS_NMEA);
    thread AdjustSpeed(SetSpeeds);
    //thread ControlSupervisor(ControlSwitcher);
    //thread PS3(JoystickTest);
    thread NetworkControl(InitTCPServer);
    thread Navigate(WaypointNavigation);

    while(1)  //bad bad bad bad bad
        this_thread::sleep_for(chrono::milliseconds(500));
}

void InitTCPServer(void)
{
    printf("Starting Network Control Server...\r\n");
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
