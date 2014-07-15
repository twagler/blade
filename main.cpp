using namespace std;

#include "mower.h"

void my_handler (int signum)
{
    printf("Caught signal %d\r\n", signum);
}

int main() {

    thread readgps(InitGPSThread);

    //GPIO gpio23(23);

    //bool value;

    //value = gpio23.get_value();



    //signal(SIGINT, my_handler);  //doesn't work
    thread NetworkControl;
    //thread RangeSensors(InitUltraServer);
    thread ReceiveGPS(ReadGPS_NMEA);
    thread AdjustSpeed(SetSpeeds);
    thread ControlSupervisor(ControlSwitcher);
    thread PS3(JoystickTest);

    //NetworkControl = thread(InitTCPServer);

    while(1)  //bad bad bad bad bad
        this_thread::sleep_for(chrono::milliseconds(500));

    //if(ReceiveGPS.joinable())
    //    ReceiveGPS.join();
    //if(AdjustSpeed.joinable())
    //    AdjustSpeed.join();
    //if(ControlSupervisor.joinable())
    //    ControlSupervisor.join();


    //delete gpio23;

    //return(0);

}

void InitTCPServer(void)
{
    printf("Starting Network Control Server...\r\n");
    ControlServer cs;
    cs.run();
    while(true);
}
void InitGPSThread(void)
{
    printf("Starting GPS Parsing Thread...\r\n");
    parser.read_serial();  //while(true) inside
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
            Navigate.join();

        this_thread::sleep_for(chrono::milliseconds(500));
    }
}
