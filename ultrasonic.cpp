#include "mower.h"
#include "ultrasonic.h"

UltraSonic::UltraSonic()
{
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
