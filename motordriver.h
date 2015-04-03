#ifndef MOTORDRIVER_H
#define MOTORDRIVER_H

#include "serialport.h"

#define SABERTOOTH_ADDRESS 130
#define PACKET_LENGTH 8

#define LEFT_MOTOR_FORWARD      0
#define LEFT_MOTOR_BACKWARD     1
#define RIGHT_MOTOR_FORWARD     4
#define RIGHT_MOTOR_BACKWARD    5
#define CHANGE_BAUD_COMMAND     15

class MotorDriver
{
public:
    MotorDriver();
    void setSpeeds(char, char);
    void sendSpeeds();
    void setMotorEnable(bool);
    char getLeftSpeed();
    char getRightSpeed();
    bool getEnable();
    void setBaudRate(int, char);

    struct SabertoothPacket
    {
        unsigned char address;
        unsigned char command;
        unsigned char data;
        unsigned char checksum;
    };

private:
    signed char myLeftSpeed;
    signed char myRightSpeed;
    bool myEnable;
    SerialPort myPort;

};

#endif // MOTORDRIVER_H
