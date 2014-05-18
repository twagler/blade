using namespace std;

#include "mower.h"

void TriggerUltras(unsigned char channel) {

    //pull whichever IO pin low for at least 20usec
}

void UpdateUltraData()
{

    unsigned char value;

    while(1)
    {
        for(unsigned char i=0;i<NUM_ULTRAS;i++)
        {
            TriggerUltras(i);
            //wait on UART for "Rxxx" throw away the R, convert to int
            //std::cin >> "R" >> value;
            value = 0;//replace 0 with whatever is read
            uSonic[i] = (uSonic[i]*3+value)/4;  //running average
        }
        this_thread::sleep_for(chrono::milliseconds(500));
        cout << "Refreshed Sensors...\r\n";
    }
}
