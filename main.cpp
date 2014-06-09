using namespace std;

#include "mower.h"

void my_handler (int signum)
{
    printf("Caught signal %d\r\n", signum);
}

int main() {


    signal(SIGINT, my_handler);  //doesn't work
    thread NetworkControl;
    thread RangeSensors(InitUltraServer);
    thread ReceiveGPS(ReadGPS);
    thread AdjustSpeed(SetSpeeds);
    thread ControlSupervisor(ControlSwitcher);
    thread PS3(JoystickTest);

    NetworkControl = thread(InitTCPServer);

    while(1)  //bad bad bad bad bad
        this_thread::sleep_for(chrono::milliseconds(500));



    //if(ReceiveGPS.joinable())
    //    ReceiveGPS.join();
    //if(AdjustSpeed.joinable())
    //    AdjustSpeed.join();
    //if(ControlSupervisor.joinable())
    //    ControlSupervisor.join();


    return(0);

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
    thread Navigate;
    while(true)
    {
        if(Autonomous)
            Navigate = thread(WaypointNavigation);

        while(Autonomous)
            this_thread::sleep_for(chrono::milliseconds(500));

        if(Navigate.joinable())
            Navigate.join();

        this_thread::sleep_for(chrono::milliseconds(500));
    }
}
