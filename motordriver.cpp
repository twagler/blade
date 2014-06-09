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

    //
    // Open the serial port.
    //
    const char* const SERIAL_PORT_DEVICE = "/dev/tty" ;
    using namespace LibSerial ;
    SerialStream serial_port ;
    serial_port.Open( SERIAL_PORT_DEVICE ) ;
    if ( ! serial_port.good() )
    {
        std::cerr << "Error: Could not open serial port"
                  << SERIAL_PORT_DEVICE
                  << "\r\n";
        //exit(1) ;
    }
    //
    // Set the baud rate of the serial port.
    //
    serial_port.SetBaudRate( SerialStreamBuf::BAUD_115200 ) ;
    if ( ! serial_port.good() )
    {
        std::cerr << "Error: Could not set the baud rate.\r\n";
        //exit(1) ;
    }
    //
    // Set the number of data bits.
    //
    serial_port.SetCharSize( SerialStreamBuf::CHAR_SIZE_8 ) ;
    if ( ! serial_port.good() )
    {
        std::cerr << "Error: Could not set the character size.\r\n";
        //exit(1) ;
    }
    //
    // Disable parity.
    //
    serial_port.SetParity( SerialStreamBuf::PARITY_NONE ) ;
    if ( ! serial_port.good() )
    {
        std::cerr << "Error: Could not disable the parity.\r\n";
        //exit(1) ;
    }
    //
    // Set the number of stop bits.
    //
    serial_port.SetNumOfStopBits( 1 ) ;
    if ( ! serial_port.good() )
    {
        std::cerr << "Error: Could not set the number of stop bits.\r\n";
        //exit(1) ;
    }
    //
    // Turn on hardware flow control.
    //
    serial_port.SetFlowControl( SerialStreamBuf::FLOW_CONTROL_NONE ) ;
    if ( ! serial_port.good() )
    {
        std::cerr << "Error: Could not use hardware flow control.\r\n";
        //exit(1) ;
    }

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

    my_comm_port.write(buffer, sizeof buffer);

    //if(my_comm_port.serial_write(buffer) != 8)
    //    printf("motor driver serial write error\r\n");

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
