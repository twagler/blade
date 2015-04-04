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
    myPort = SerialPort("/dev/ttyO1",9600);

}
void MotorDriver::setSpeeds(signed char leftspeed, signed char rightspeed)
{
    myLeftSpeed = leftspeed;
    myRightSpeed = rightspeed;
}

void MotorDriver::sendSpeeds()
{
    unsigned char buffer[PACKET_LENGTH];
    SabertoothPacket Packet_Left;
    SabertoothPacket Packet_Right;

    Packet_Left.address = 130;
    Packet_Right.address = 130;

    //send leftspeed
    if(myLeftSpeed >=0)  //drive forward
    {
        Packet_Left.command = LEFT_MOTOR_FORWARD;
        Packet_Left.data = (unsigned char)myLeftSpeed;
    }
    else //drive backward
    {
        Packet_Left.command = LEFT_MOTOR_BACKWARD;
        Packet_Left.data = (unsigned char)(abs(myLeftSpeed));

    }
    if(myRightSpeed >=0) //drive forward
    {
        Packet_Right.command = RIGHT_MOTOR_FORWARD;
        Packet_Right.data = (unsigned char)myRightSpeed;
    }
    else //drive backward
    {
        Packet_Right.command = RIGHT_MOTOR_BACKWARD;
        Packet_Right.data = (unsigned char)(abs(myRightSpeed));
    }

    Packet_Left.checksum = (Packet_Left.address + Packet_Left.command
                            + Packet_Left.data) & 0b01111111;

    Packet_Right.checksum = (Packet_Right.address + Packet_Right.command
                             + Packet_Right.data) & 0b01111111;

    buffer[0] = Packet_Left.address;
    buffer[1] = Packet_Left.command;
    buffer[2] = Packet_Left.data;
    buffer[3] = Packet_Left.checksum;
    buffer[4] = Packet_Right.address;
    buffer[5] = Packet_Right.command;
    buffer[6] = Packet_Right.data;
    buffer[7] = Packet_Right.checksum;

    int n = 0;
    n = write(myPort.serial_fd,buffer, sizeof buffer);

    if(n != 8)
        printf("motor driver serial write error\r\n");

}

void MotorDriver::setBaudRate(int baud, unsigned char address)
{
    SabertoothPacket BaudPacket;

    BaudPacket.address = address;
    BaudPacket.command = CHANGE_BAUD_COMMAND;
    BaudPacket.data = baud;
    BaudPacket.checksum = (BaudPacket.address + BaudPacket.command
                           + BaudPacket.data) & 0b01111111;

}

void MotorDriver::setMotorEnable(bool status)
{
    this->myEnable = status;
}

signed char MotorDriver::getLeftSpeed()
{
    return myLeftSpeed;
}

signed char MotorDriver::getRightSpeed()
{
    return myRightSpeed;
}

bool MotorDriver::getEnable()
{
    return myEnable;
}
