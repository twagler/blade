#ifndef MOTORDRIVER_H
#define MOTORDRIVER_H

#define SABERTOOTH_ADDRESS 130

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

private:
    char myLeftSpeed;
    char myRightSpeed;
    bool myEnable;
};

#endif // MOTORDRIVER_H
