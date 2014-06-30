#include "libgpio.h"

GPIO::GPIO(unsigned char pin_num)
{
    this->my_pin_num = pin_num;
    this->my_direction = INPUT;
    this->my_value = 0;

    this->export_gpio();
}

GPIO::GPIO(unsigned char pin_num, bool direction)
{
    this->my_pin_num = pin_num;
    this->my_direction = direction;
    this->my_value = 1;

    this->export_gpio();
}

GPIO::~GPIO()
{

}

bool GPIO::set_value(bool value)
{
    if(this->my_direction==INPUT)
    {
        //throw error
        return 1;
    }
    else
    {
        //set the value
        this->my_value = value;
        return 0;
    }
}

bool GPIO::get_value()
{
    return 0;
}

unsigned char GPIO::get_pin_num()
{
    return this->my_pin_num;
}

int GPIO::export_gpio()
{
    int statusVal = -1;
    int fd;
    char buffer[3];  //buffer for unsigned char to c-string
    string exportStr = "/sys/class/gpio/export";
    string numStr;
    int length;



    //string numStr = buffer;


    fd = statusVal = open(exportStr.c_str(),  O_WRONLY|O_SYNC);
    if (statusVal < 0){
        perror("could not open SYSFS GPIO export device");
        exit(1);
    }

    //stringstream ss;
    //ss << this->my_pin_num.toString();
    //string numStr = ss.str();

    //numStr = this->my_pin_num;
    //sprintf(numStr,"%c",this->my_pin_num);

    length = sprintf(buffer,"%u",this->my_pin_num);
    numStr = buffer;
    statusVal = write(fd, buffer, length);
    if (statusVal < 0){
        perror("could not write to SYSFS GPIO export device");
        exit(1);
    }

    statusVal = close(fd);
    if (statusVal < 0){
        perror("could not close SYSFS GPIO export device");
        exit(1);
    }

    return statusVal;
}
