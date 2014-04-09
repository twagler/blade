#include "motordriver.h"

MotorDriver::MotorDriver()
{
    myLeftSpeed = 0;
    myRightSpeed = 0;
    myEnable = 0;
}
void MotorDriver::SetSpeeds(char adjustment, char targetspeed)
{
    char leftspeed, rightspeed;
    leftspeed = targetspeed + adjustment;
    rightspeed = targetspeed - adjustment;


    if (leftspeed < MIN_SPEED)                 //cap min motor speed
        leftspeed = MIN_SPEED;
    else if (leftspeed>MAX_SPEED)
        leftspeed = MAX_SPEED;                  //cap max motor speed

    if (rightspeed<MIN_SPEED)
        rightspeed = MIN_SPEED;
    else if (rightspeed>MAX_SPEED)
        rightspeed = MAX_SPEED;

    if(this->myEnable)
    {
        this->myLeftSpeed = leftspeed;
        this->myRightSpeed = rightspeed;
    }

}

void MotorDriver::SendSpeeds(char leftspeed, char rightspeed)
{
    //write leftspeed  to Sabertooth
    //write rightspeed to Sabertooth

}

void MotorDriver::SetMotorEnable(bool status)
{
    this->myEnable = status;
}
