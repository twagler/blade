#include "mower.h"

/* This thread opens the /dev/js* device and continuously
 * polls for events.  If an event is found the thread
 * handles that button as defined...
 */
void JoystickTest()
{
    printf("Starting Joystick thread...\r\n");

    Joystick joystick(0);
    // Ensure that it was found and that we can use it
    if (!joystick.isFound())
    {
        printf("Joystick open failed...\r\n"); //print error
        exit(1);    //exit program if joystick is not found
        //this is probably a bad idea
    }

    motors.setMotorEnable(true);    //enable the motors
    //shouldn't do this here

    signed char leftspeed, rightspeed;     //speed values for motors L&R
    JoystickEvent event;            //linux event object
    while (true)    //loop loop loop...
    {
        if (joystick.sample(&event))  //if we got a bite
        {
            //if that bite is a button and not the first one
            if (event.isButton() && !event.isInitialState())
            {
                switch(event.number)  //which one
                {
                case PS:    //playstation button
                    if(event.value==0)  //only trigger on the down-press
                        Autonomous = !Autonomous; //toggle auto

                    printf("PS is: "); //print button label
                    break;
                case TRIANGLE:  //triangle button
                    printf("Triangle is: "); //print button label
                    break;
                case CIRCLE:  //circle button
                    printf("Circle is: ");  //print button label
                    break;
                case SQUARE: //square button
                    printf("Square is: ");  //print button label
                    break;
                case X:  //x... EX... ecks...?  button
                    printf("X is: ");  //print button label
                    break;
                default:
                    printf("Button %u is: ", event.number); //everything else
                    break;
                }
                //print button status
                printf("%s\r\n", event.value == 0 ? "up" : "down");
            }
            //tie left wheel to axis 1 and right wheel to axis 3
            else if (event.isAxis() && !event.isInitialState() &&
                     (event.number == LEFT_STICK || event.number == RIGHT_STICK)
                     && !Autonomous)
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

                /*  Apparently this driver works normally except
                 * when you push the stick all the way forward.
                 * In this case the value climbs from 0,1,2,3,4...
                 * all the way up to 127.  Push just a little further
                 * and you get -128... Silly, right?
                 * Hence, bug workaround...
                 */

                //BUG WORKAROUND
                if(leftspeed == -128)
                    leftspeed= 127;
                if (rightspeed == -128)
                    rightspeed = 127;
                //BUG WORKAROUND


                //if the motors are turned on, go ahead and set the
                //speeds dictated by the sticks.
                //by the way, print out the values for giggles...
                if (motors.getEnable())  //are the motors on?
                {
                    motors.setSpeeds(leftspeed, rightspeed);  //set the values
                    //print them
                    //printf("Speeds: (%i,%i)\r\n",
                    //       (int)leftspeed, (int)rightspeed);
                    motors.sendSpeeds();  //send the to the sabertooth
                }
            }
        }
        //sleep the thread for a millisecond
        //so the CPU doesn't shit the bed...
        this_thread::sleep_for(chrono::milliseconds(1));
    }
}
