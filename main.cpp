using namespace std;
#include "mower.h"

int main() {

    //char targetspeed = 65;

    thread RangeSensors(UpdateUltraData);
    thread ReceiveGPS(ReadGPS);
    thread Navigate(WaypointNavigation);
    thread AdjustSpeed(SetSpeeds);

    while(1);
}
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
        cout << "Read: " << gps.getLatitude() << "," << gps.getLongitude() << "," << gps.getTime() << "\r\n";
        gps_lock.unlock();
        cv_gps.notify_one();
        this_thread::sleep_for(chrono::milliseconds(500));
    }
    GPSfile.close();
}

void SetSpeeds()
{
    cout << "Starting speed setting thread...\r\n";

    MotorDriver motors;

    char leftspeed, rightspeed;

    while(1){

        unique_lock<mutex> lk_drive(drive_lock);
        cv_gps.wait(lk_drive);

        leftspeed = targetspeed + adjustment;
        rightspeed = targetspeed - adjustment;

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

        cout << "Motors: (" << leftspeed << "," << rightspeed << ")\r\n";
    }
}
