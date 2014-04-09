#ifndef MOTORDRIVER_H
#define MOTORDRIVER_H

#define MIN_SPEED -126
#define MAX_SPEED  126

class MotorDriver
{
public:
    MotorDriver();
    void SetSpeeds(char, char);
    void SendSpeeds(char, char);
    void SetMotorEnable(bool);

private:
    char myLeftSpeed;
    char myRightSpeed;
    bool myEnable;
};

#endif // MOTORDRIVER_H
