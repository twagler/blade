using namespace std;

#include "mower.h"

//use an array of waypoints for now.  A more robust mapping mechanism will be required
float LATwaypoint[] = {37.971417,   37.971495,  37.971497,  37.971432,  37.971417,  37.971495,  37.971497,  37.971432};
float LONwaypoint[] = {-87.529813, -87.529810, -87.529698, -87.529695, -87.529813, -87.529810, -87.529698, -87.529695};

int uSonic[NUM_ULTRAS];

mutex gps_lock;
condition_variable cv_gps;
GPS gps;

mutex drive_lock;
condition_variable cv_drive;

bool Autonomous = false;
bool first = true;
char adjustment;
char targetspeed = 55;

MotorDriver motors;

void ReadGPS()
{
    cout << "Starting GPS Reception thread...\r\n";
    float lat, lon;
    int time;
    char trash;

    ifstream GPSfile;
    GPSfile.open("input.txt");

    while(1)
    {
        GPSfile >> lat >> trash >> lon >> trash >> time;

        gps_lock.lock();
        gps.setLatitude(lat);
        gps.setLongitude(lon);
        gps.setTime(time);
        gps_lock.unlock();
        cv_gps.notify_one();
        this_thread::sleep_for(chrono::milliseconds(500));
    }
    GPSfile.close();
}

void SetSpeeds()
{
    cout << "Starting speed setting thread...\r\n";

    char leftspeed, rightspeed;

    while(true){

        unique_lock<mutex> lk_drive(drive_lock);
        cv_drive.wait(lk_drive);

        leftspeed = targetspeed - adjustment;
        rightspeed = targetspeed + adjustment;

        lk_drive.unlock();

        if (leftspeed < MIN_SPEED)                 //cap min motor speed
            leftspeed = MIN_SPEED;
        else if (leftspeed>MAX_SPEED)
            leftspeed = MAX_SPEED;                  //cap max motor speed

        if (rightspeed<MIN_SPEED)
            rightspeed = MIN_SPEED;
        else if (rightspeed>MAX_SPEED)
            rightspeed = MAX_SPEED;

        if (motors.getEnable())
        {
            motors.setSpeeds(leftspeed, rightspeed);
        }

        motors.sendSpeeds();

        cout << "Motors: (" << (int)leftspeed << ","
             << (int)rightspeed << ")\r\n";
    }
}

void ControlCommands()
{
    //Set up TCP/IP server to accept commands across the network

    /*  crap...
    fstream CommandStream;
    int command;
    CommandStream.open("commands.txt");
    while(1)
    {
        CommandStream >> command;

        switch(command)
        {
        case AUTONOMOUS:
            //enable thread WaypointNavigation
            break;

        case MANUAL:
            //enable thread JoystickControl
            break;

        default:
            break;

        }
    }*/
}

void ControlSwitcher()
{
    //initially we'll always depend on the joystick having control
    //we'll change this later to accept commands via network
    while(1)
    {
        thread PS3(JoystickTest);
        while(!Autonomous)
            this_thread::sleep_for(chrono::milliseconds(500));
        PS3.join();
        thread Navigate(WaypointNavigation);
        while(Autonomous)
            this_thread::sleep_for(chrono::milliseconds(500));
        Navigate.join();

    }
}
