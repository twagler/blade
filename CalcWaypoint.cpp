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



    /*Alan's bitchin' algorithm of sexiness:

    Assume we've already generated our boundaries.  It is a closed shape.

    First step: sanity check.  Are we in-bounds?  If not, STOP.

    Run function to identify Boustrophedon sections, assuming no obsticles.
        take into account known objects from previous mowings.


    Execution:
    Identify current position

    If on a Boustrophedon area:
        Are we on a corner?
            If so, Boustrophedon that shit.
            If not, divide Boustrophedon into 2+ Boustrophedons and Boustrophedon that shit

    Else if not on a Boustrophedon area:
        Spiral out until an object, boundary, or Boustrophedon is hit.
        If an object is hit:
            Attempt to encircle object to identify scope
            If encircling is not possible, continue until a boundary or Boustrophedon is hit.
            Return to spot of initial object detection and attempt to encircle the other way.
            Regenerate Boustrophedons and restart execution
        If a boundary or Boustrophedon is hit
            Regenerate Boustrophedons and restart execution


    If uncut area is small (need to define 'small'), do not circle, just try to cut instead.



    Need to track the following in data structure for each square of grass:
        uncut
        cut
        obstical
        boundary
        out of bounds.
        occupied by mower

        area of mower != area of cutting

        size of mower should be variable.




    */
}
