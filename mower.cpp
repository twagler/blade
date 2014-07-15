using namespace std;

#include "mower.h"

//use an array of waypoints for now.  A more robust mapping mechanism will be required
double LATwaypoint[] = {37.971417,   37.971495,  37.971497,  37.971432,  37.971417,  37.971495,  37.971497,  37.971432};
double LONwaypoint[] = {-87.529813, -87.529810, -87.529698, -87.529695, -87.529813, -87.529810, -87.529698, -87.529695};

//vector<GPS> waypoints;

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

NMEAParser parser;

void ReadGPS_RTKLIB()
{
    cout << "Starting RTKLIB GPS Reception thread...\r\n";
    float lat, lon;
    int time;
    char trash;

    ifstream GPSfile;
    GPSfile.open("input.txt");

    while(true)
    {
        GPSfile >> lat >> trash >> lon >> trash >> time;

        gps_lock.lock();
        gps.setLatitude(lat);
        gps.setLongitude(lon);
        //gps.setTime(time);
        gps_lock.unlock();
        cv_gps.notify_one();
        this_thread::sleep_for(chrono::milliseconds(500));
    }
    GPSfile.close();
}

void ReadGPS_NMEA()
{
    cout << "Starting NMEA GPS Reception thread...\r\n";

    while(true)
    {
        gps_lock.lock();
        gps = parser.GetActualGPSInfo();
        gps_lock.unlock();
        cv_gps.notify_one();

        this_thread::sleep_for(chrono::milliseconds(500));
    }
}

void SetSpeeds()
{
    cout << "Starting speed setting thread...\r\n";

    char leftspeed, rightspeed;

    while(true)
    {
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

        if(!Autonomous)
        {
            leftspeed = 0;
            rightspeed = 0;
        }

        if (motors.getEnable())
        {
            motors.setSpeeds(leftspeed, rightspeed);
            motors.sendSpeeds();
        }
        cout << "Motors: (" << (int)leftspeed << ","
             << (int)rightspeed << ")\r\n";
    }
}

float gps_distance(GPS one, GPS two)
{
    double dlong = (two.getLongitude() - one.getLongitude())*d2r;
    double dlat = (two.getLatitude() - one.getLatitude())*d2r;
    double a1 = sin(dlat/2.0);
    double a2 = sin(dlong/2.0);
    double a = (a1*a1) + cos(one.getLatitude()*d2r) *
            cos(two.getLatitude()*d2r) * (a2*a2);
    double c = 2 * atan2(sqrt(a), sqrt(1-a));
    double d = 2.54 * 12 * 5280 * 3956 * c; //cm, in, ft, scaler

    return d;
}


