#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#define NUM_ULTRAS 5

class UltraSonic
{
public:
    UltraSonic();
    void updateUltra();

private:
    int my_serial_fd;
    unsigned char sensor_array[NUM_ULTRAS];
    void trigger_sensors();

};

#endif // ULTRASONIC_H
