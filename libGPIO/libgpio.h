#ifndef LIBGPIO_H
#define LIBGPIO_H

#define INPUT   0
#define OUTPUT  1

#define LOW     0
#define HIGH    1

#include <string>
#include <sstream>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fcntl.h>

using namespace std;

class GPIO
{
public:
    GPIO(unsigned char);
    GPIO(unsigned char, bool);
    ~GPIO();

    bool set_value(bool);
    bool get_value();

    bool set_direction(bool);
    bool get_direction();

    bool set_pin_num(unsigned char);
    unsigned char get_pin_num();

private:
    bool my_value;
    bool my_direction;
    unsigned char my_pin_num;
    int export_gpio();

};

#endif // LIBGPIO_H
