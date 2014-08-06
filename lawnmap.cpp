#include "lawnmap.h"
#include <vector>
#include <ctime>

using namespace std;


LawnMap::LawnMap()
{
    //not used
}

LawnMap::LawnMap(vector<double[2]>){
    //this needs to take the initial boundary coordinates and generate a map w/ them
}

int LawnMap::boundaryCheck(){
        //This should check the map and ensure that the current position is in bouonds.

        //If it is in bounds return 1

        //If it is out of bounds return 0
        return 0;  //placeholder
}

int LawnMap::atWaypoint(){
    //This function compares where we are with where we are going.  If they are the same, return 1.  Otherwise return 0.
    return 0; //placeholder
}

int LawnMap::canTurn(){
    //This function checks to see if we can turn.  If so, we are not done with the boustrophedon, return 1.  If not, we are done, return 0.
    return 0; //placeholder
}

void LawnMap::nextRow(){
    //This function sets the new waypoint and places the old waypoint in the fromGPS variables.
}
