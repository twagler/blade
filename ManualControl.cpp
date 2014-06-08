using namespace std;

#include "mower.h"

void JoystickTest()
{
    printf("Starting Joystick thread...\r\n");

    Joystick joystick(0);
    // Ensure that it was found and that we can use it
    if (!joystick.isFound())
    {
        printf("open failed.\n");
        //exit(1);
    }

    motors.setMotorEnable(true);

    char leftspeed, rightspeed;
    JoystickEvent event;
    while (true)
    {
        if (joystick.sample(&event))
        {
            if (event.isButton() && !event.isInitialState())
            {
                switch(event.number)
                {
                case PS:
                    if(event.value==0)
                        Autonomous = !Autonomous;
                    printf("PS is: ");
                    break;
                case TRIANGLE:
                    printf("Triangle is: ");
                    break;
                case CIRCLE:
                    printf("Circle is: ");
                    break;
                case SQUARE:
                    printf("Square is: ");
                    break;
                case X:
                    printf("X is: ");
                    break;
                default:

                    printf("Button %u is: ", event.number);
                    break;
                }
                printf("%s\r\n", event.value == 0 ? "up" : "down");
            }
            //tie left wheel to axis 1 and right wheel to axis 3
            else if (event.isAxis() && !event.isInitialState() &&
                     (event.number == 1 || event.number ==3) && !Autonomous)
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
                if (rightspeed == -128)
                    rightspeed = 127;
                //BUG WORKAROUND

                if (motors.getEnable())
                {
                    motors.setSpeeds(leftspeed, rightspeed);
                    printf("Speeds: (%i,%i)\r\n",
                           (int)leftspeed, (int)rightspeed);
                    motors.sendSpeeds();
                }
            }
        }
        this_thread::sleep_for(chrono::milliseconds(1));
    }
}
