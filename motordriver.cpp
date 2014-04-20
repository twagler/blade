using namespace std;
#include "mower.h"
#include "motordriver.h"


MotorDriver::MotorDriver()
{
    /*At some point we need to add an initialization argument that says
     * which port the Sabertooth motor driver is attached to.
     * We also need to properly initialize the baud rate, partiy, etc.
     */
    myLeftSpeed = 0;
    myRightSpeed = 0;
    myEnable = 0;
}
void MotorDriver::setSpeeds(char leftspeed, char rightspeed)
{
    myLeftSpeed = leftspeed;
    myRightSpeed = rightspeed;
}

void MotorDriver::sendSpeeds()
{
    struct SabertoothPacket
    {
        char address;
        char command;
        char data;
        char checksum;
    };

    SabertoothPacket Packet_Left;
    SabertoothPacket Packet_Right;

    Packet_Left.address = SABERTOOTH_ADDRESS;
    Packet_Right.address = SABERTOOTH_ADDRESS;

    //send leftspeed
    if(myLeftSpeed >=0)  //drive forward
    {
        Packet_Left.command = 0;
        Packet_Left.data = myLeftSpeed;
    }
    else //drive backward
    {
        Packet_Left.command = 1;
        Packet_Left.data = abs(myLeftSpeed);

    }
    if(myRightSpeed >=0) //drive forward
    {
        Packet_Right.command = 4;
        Packet_Right.data = myRightSpeed;
    }
    else //drive backward
    {
        Packet_Right.command = 5;
        Packet_Right.data = abs(myRightSpeed);
    }

    Packet_Left.checksum = (Packet_Left.address + Packet_Left.command
                            + Packet_Left.data) & 0b01111111;

    Packet_Right.checksum = (Packet_Right.address + Packet_Right.command
                             + Packet_Right.data) & 0b01111111;

    cout << Packet_Left.address;
    cout << Packet_Left.command;
    cout << Packet_Left.data;
    cout << Packet_Left.checksum;
    cout << Packet_Right.address;
    cout << Packet_Right.command;
    cout << Packet_Right.data;
    cout << Packet_Right.checksum;
}

void MotorDriver::setMotorEnable(bool status)
{
    this->myEnable = status;
}

char MotorDriver::getLeftSpeed()
{
    return myLeftSpeed;
}

char MotorDriver::getRightSpeed()
{
    return myRightSpeed;
}

bool MotorDriver::getEnable()
{
    return myEnable;
}
