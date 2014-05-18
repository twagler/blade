using namespace std;

#include "mower.h"

void JoystickTest()
{

    Joystick joystick(0);
    // Ensure that it was found and that we can use it
    if (!joystick.isFound())
    {
        printf("open failed.\n");
        //exit(1);
    }

    MotorDriver motors;
    motors.setMotorEnable(true);

    char leftspeed, rightspeed;

    while (true)
    {
        JoystickEvent event;
        if (joystick.sample(&event))
        {
            if (event.isButton() && !event.isInitialState())
            {
                switch(event.number)
                {
                case PS:
                    printf("Toggling Manual/Autonomous\r\n");
                    break;
                case TRIANGLE:
                    printf("Toggling Blades ON/OFF\r\n");
                    break;
                case CIRCLE:
                    printf("You hit circle\r\n");
                    break;
                case SQUARE:
                    printf("You hit square\r\n");
                    break;
                case X:
                    printf("You hit x\r\n");
                    break;
                default:

                    printf("Button %u is %s\n", event.number,
                           event.value == 0 ? "up" : "down");
                    break;
                }
            }
            //tie left wheel to axis 1 and right wheel to axis 3
            else if (event.isAxis() && !event.isInitialState() &&
                     (event.number == 1 || event.number ==3))
            {
                switch(event.number)
                {
                case LEFT_STICK:
                    leftspeed = (event.value >> 8)*-1;
                    break;
                case RIGHT_STICK:
                    rightspeed = (event.value >> 8)*-1;
                    break;
                default:
                    break;

                }

                //BUG WORKAROUND
                if(leftspeed == -128)
                    leftspeed= 127;
                else if (rightspeed == -128)
                    rightspeed = 127;
                //BUG WORKAROUND

                if (motors.getEnable())
                {
                    motors.setSpeeds(leftspeed, rightspeed);
                    printf("Speeds: (%i,%i)\r\n",
                           (int)leftspeed, (int)rightspeed);
                }
            }
        }
    }
}
