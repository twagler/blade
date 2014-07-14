#include "mower.h"
#include "ultrasonic.h"

UltraSonic::UltraSonic()
{
    for (int i=0;i<NUM_ULTRAS;i++)
        sensor_array[i] = 0;
}

void UltraSonic::trigger_sensors()
{
    //pull whichever IO pin low for at least 20usec
}

void UltraSonic::updateUltra()
{
    unsigned char value;
    trigger_sensors();
    for(unsigned char i=0;i<NUM_ULTRAS;i++)
    {
        //wait on UART for "Rxxx" throw away the R, convert to int
        //std::cin >> "R" >> value;
        value = 0;//replace 0 with whatever is read
        sensor_array[i] = (sensor_array[i]*3+value)/4;  //running average
    }
}
