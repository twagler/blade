#include "mower.h"
#define ROBOT_WIDTH .000005

/*
theta = arctan((cornery[2] - cornery[1])/(cornerx[2]-cornerx[1]));
xComponent = ROBOT_WIDTH*sin(theta);
yComponent = ROBOT_WIDTH*cos(theta);

//next calc

nextwaypointx = threewaypointsagox + xcomponent
nextwaypointy = threewaypointsagoy + ycomponent

*/

void CalcWaypoint()
{
    //if moving up/down
            //LATwaypoint[incwaypoint(2)]=LATwaypoint[incwaypoint(1)]+ROBOT_WIDTH;
            //LONwaypoint[incwaypoint(2)]=LONwaypoint[incwaypoint(1)];
    //if moving left right
            //LATwaypoint[incwaypoint(2)]=LATwaypoint[incwaypoint(1)];
            //LONwaypoint[incwaypoint(2)]=LONwaypoint[decwaypoint(1)];


}
