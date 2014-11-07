#include "mower.h"
#include "lawnmap.h"
#include <exception>
#define ROBOT_WIDTH .000005

/*
theta = arctan((cornery[2] - cornery[1])/(cornerx[2]-cornerx[1]));
xComponent = ROBOT_WIDTH*sin(theta);
yComponent = ROBOT_WIDTH*cos(theta);

//next calc

nextwaypointx = threewaypointsagox + xcomponent
nextwaypointy = threewaypointsagoy + ycomponent

*/
void CalcWaypoint(){
    //not used, to be deleted eventually
}

void CalcWaypoint(LawnMap &map)
{
    //if moving up/down
            //LATwaypoint[incwaypoint(2)]=LATwaypoint[incwaypoint(1)]+ROBOT_WIDTH;
            //LONwaypoint[incwaypoint(2)]=LONwaypoint[incwaypoint(1)];
    //if moving left right
            //LATwaypoint[incwaypoint(2)]=LATwaypoint[incwaypoint(1)];
            //LONwaypoint[incwaypoint(2)]=LONwaypoint[decwaypoint(1)];



    /*Alan's bitchin' algorithm of sexiness:

    Map should have been generated previously and passed in as the variable 'map'.

    First step: sanity check.  Are we in-bounds?  If not, STOP. */

    //if(! map.boundaryCheck() ){
    //    throw std::exception(); //Mower out of bounds, Correct course
   // }

    /* Run function to identify Boustrophedon sections, assuming no obsticles.
        take into account known objects from previous mowings.


    Execution:
    Identify current position

    Did we just get to where we were going?
    Check if toGPS is the same as currentGPS. */

    //if( map.atWaypoint()) {
        //we have finished our run, should either be finished with boustrophedon or turning around
        //can we turn?
        //if( map.canTurn()){
            //turn chickever way we need to.
            //map.nextRow();

        //}
    //}

    /*If on a Boustrophedon area
        If finished mowing the Boustrophedon, move on
        Are we on a corner?

        Are we on an edge?

        Are we in the middle?
            Obsticle?
            In transit?
            Is Boustrophedon finished?  If so, move on.
            If not, need to mow it.

            If so, Boustrophedon that shit.
            If not, go to a corner

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

