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
    my_comm_port.serial_initialize("/dev/tty");
}
void MotorDriver::setSpeeds(char leftspeed, char rightspeed)
{
    myLeftSpeed = leftspeed;
    myRightSpeed = rightspeed;
}

void MotorDriver::sendSpeeds()
{
    char buffer[PACKET_LENGTH];
    SabertoothPacket Packet_Left;
    SabertoothPacket Packet_Right;

    Packet_Left.address = SABERTOOTH_ADDRESS;
    Packet_Right.address = SABERTOOTH_ADDRESS;

    //send leftspeed
    if(myLeftSpeed >=0)  //drive forward
    {
        Packet_Left.command = LEFT_MOTOR_FORWARD;
        Packet_Left.data = myLeftSpeed;
    }
    else //drive backward
    {
        Packet_Left.command = LEFT_MOTOR_BACKWARD;
        Packet_Left.data = abs(myLeftSpeed);

    }
    if(myRightSpeed >=0) //drive forward
    {
        Packet_Right.command = RIGHT_MOTOR_FORWARD;
        Packet_Right.data = myRightSpeed;
    }
    else //drive backward
    {
        Packet_Right.command = RIGHT_MOTOR_BACKWARD;
        Packet_Right.data = abs(myRightSpeed);
    }

    Packet_Left.checksum = (Packet_Left.address + Packet_Left.command
                            + Packet_Left.data) & 0b01111111;

    Packet_Right.checksum = (Packet_Right.address + Packet_Right.command
                             + Packet_Right.data) & 0b01111111;

    buffer[1] = Packet_Left.address;
    buffer[2] = Packet_Left.command;
    buffer[3] = Packet_Left.data;
    buffer[4] = Packet_Left.checksum;
    buffer[5] = Packet_Right.address;
    buffer[6] = Packet_Right.command;
    buffer[7] = Packet_Right.data;
    buffer[8] = Packet_Right.checksum;

    my_comm_port.serial_write(buffer);

    /*
    //DEBUG
    comm_port.serial_write(comm_port.Packet_Left.address);
    comm_port.serial_write()
    << "Left Packet... ";
    cout << (int)Packet_Left.address << ",";
    cout << (int)Packet_Left.command << ",";
    cout << (int)Packet_Left.data << ",";
    cout << (int)Packet_Left.checksum << endl;
    cout << "Right Packet... ";
    cout << (int)Packet_Right.address << ",";
    cout << (int)Packet_Right.command << ",";
    cout << (int)Packet_Right.data << ",";
    cout << (int)Packet_Right.checksum << endl;
    //DEBUG
    */
}

void MotorDriver::setBaudRate(int baud, char address)
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
