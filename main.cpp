using namespace std;
#include "mower.h"
#include <thread>

int main() {


    MotorDriver motors;
    //char targetspeed = 65;

    thread RangeSensors(UpdateUltraData);
    thread ReceiveGPS(ReadGPS);
    thread Navigate(WaypointNavigation);
    //thread AdjustSpeed(motors.SetSpeeds(&adjustment, targetspeed));

    while(1);
}

void ReadGPS()
{
    cout << "Starting GPS Reception thread...\r\n";
    float lat, lon;
    int time;
    char trash;

    while(1)
    {
        cout << "Enter coordinates: X,Y,T: ";
        cin >> lat >> trash >> lon >> trash >> time;

        gps_lock.lock();
        gps.setLatitude(lat);
        gps.setLongitude(lon);
        gps.setTime(time);
        ready = true;
        cout << "Entered: " << gps.getLatitude() << "," << gps.getLongitude() << "," << gps.getTime() << "\r\n";
        gps_lock.unlock();

    }

}
