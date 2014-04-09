#ifndef MOTORDRIVER_H
#define MOTORDRIVER_H

#define MIN_SPEED -127
#define MAX_SPEED  127

class MotorDriver
{
public:
    MotorDriver();
    void SetSpeeds(char);
    void SendSpeeds(char, char);
    void SetMotorEnable(bool);

private:
    char myLeftSpeed;
    char myRightSpeed;
    bool enabled;
};

#endif // MOTORDRIVER_H
