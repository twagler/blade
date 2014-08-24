#include "lawnmap.h"
#include <vector>
#include <ctime>

using namespace std;


LawnMap::LawnMap()
{
    //not used
}

LawnMap::LawnMap(vector<double[2]> vList){
    //this needs to take the initial boundary coordinates and generate a map w/ them
    //grab largest and lowest values for X and Y
    double highX;
    double highY;
    double lowX;
    double lowY;
    //this is assuming the GPS coordinates will have 7 decimal places of accurancy.  Will probably need to be changed.
    double precision = 10000000;

    //set initial values to first coordinate
    highX = vList.at(0)[0];
    highY = vList.at(0)[1];
    lowX = vList.at(0)[0];
    lowY = vList.at(0)[1];

    //walk through the elements, check is points are higher or lower.  if so, keep track of them
    for(int i = 0; i < vList; i++){
        if( vList.at(i)[0] > highX){
            //this coordinate is farther east than our current high X value
            //set highX to this coordinate
            highX = vList.at(i).[0];
        }
        if( vList.at(i)[0] < lowX){
            //this coordinate is farther west than our current low X Value
            //set lowX to this coordinate
            lowX = vList.at(i).[0];
        }
        if( vList.at(i)[1] > highY){
            //this coordinate is farther north than our current high Y Value
            //set highY to this coordinate
            highY = vList.at(i).[1];
        }
        if( vList.at(i)[1] < lowY){
            //this coordinate is farther south than our current low Y Value
            //set lowY to this coordinate
            lowY = vList.at(i).[1];
        }
    }

    //we now have the boundaries of our map and can generate it

    int xSize = (highX - lowX) * precision;
    int ySize = (highY - lowY) * precision;

    this->map ( xSize, std::vector<COORDINATE> (ySize, 0));

    for(int i = 0; i < vList; i++){
        (this->map.at(vList.at(i)[0]).at(vList.at(i)[1])).boundary = true;
    }

    //map should be generated
}

int LawnMap::boundaryCheck(){
        //This should check the map and ensure that the current position is in bouonds.

        //If it is in bounds return 1

        //If it is out of bounds return 0
        return 0;  //placeholder
}

int LawnMap::atWaypoint(){
    //This function compares where we are with where we are going.  If they are the same, return 1.  Otherwise return 0.
    double current[2];
    double to[2];
    current = this->getCurrentGPS();
    to = this->getToGPS();

    if(to[0] == current[0] && to[1] == current[1]){
       return 1;
    }
    return 0;
}

int LawnMap::canTurn(){
    //This function checks to see if we can turn.  If so, we are not done with the boustrophedon, return 1.  If not, we are done, return 0.
    //N=0, S=1, E=2, W=3
    if((this->getCurrentGPS())[0] < (this->getFromGPS())[0]){
            //going West
    }
    else if((this->getCurrentGPS()[1] < (this->getCurrentGPS()[1] ))){

    }
    //was falling asleep, need to finish this
    return 0; //placeholder
}

void LawnMap::nextRow(){
    //This function sets the new waypoint and places the old waypoint in the fromGPS variables.
}

BOUSTROPHEDON findMaxRectangle(BOUSTROPHEDON rect){
    //need to recursively call with expanded x and y coordinates

    //I really need to go over this, i was asleep when I coded it.

    int retval1;
    int retval2;
    BOUSTROPHEDON b1;
    BOUSTROPHEDON b2;

    b1 = rect;
    b2 = rect;

    if(lineGood(b1.xNE,b1.xSE++)) {
        //line  good.  keep calling and return the bigger of the two.
        b1.xNE++;
        b1.xSE++;
        b1 = findMaxRectangle(b1);
    }
    if(lineGood(b2.xNW++,b2.xNE++)) {
        //line  good.  keep calling and return the bigger of the two.
        b1.xNE++;
        b1.xSE++;
        b1 = findMaxRectangle(b1);
    }


    b2.yNW++;
    b2.yNE++;

    //need to check each and make sure line we added is good.
    //need to update size of rect

    retval1 = findMaxRectangle()
}
