#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#define NUM_ULTRAS 5

#include "libSerial/SerialStream.h"

class UltraSonic
{
public:
    UltraSonic();
    void updateUltra();

private:
    int my_serial_fd;
    unsigned char sensor_array[NUM_ULTRAS];
    void trigger_sensors();
    LibSerial::SerialStream my_comm_port;
};

#endif // ULTRASONIC_H
