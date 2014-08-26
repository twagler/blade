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

int LawnMap::boundaryCheck(double[2]){
        //This should check the map and ensure that the current position is in bouonds.
        //get dimensions of map
        int xSize = this->map.size();
        int ySize = this->map.at(0).size();



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
    double currentGPS[2] = this->getCurrentGPS();
    double toGPS[2] = this->getToGPS();
    double fromGPS[2] = this->getFromGPS();
    int position[2];

    //get the position from our current GPS coordinate

    position = this->getCoordinatesFromGPS(&currentGPS);

    if((currentGPS())[0] < (fromGPS())[0]){
        //going West, check to the North and South

        this->setToGPS(position);
    }
    else if((currentGPS()[0] > (fromGPS()[0] ))){
        //going East, check to the North and South
    }
    else if((currentGPS()[1] < (fromGPS()[1] ))){
        //going South, check to the East and West
    }
    else if((currentGPS()[1] > (fromGPS()[1] ))){
        //going North, check to the East and West
    }




    return 0; //placeholder
}

void LawnMap::nextRow(){
    //This function sets the new waypoint and places the old waypoint in the fromGPS variables.
}

BOUSTROPHEDON findMaxRectangle(BOUSTROPHEDON rect){
    //need to recursively call with expanded x and y coordinates

    int retval1;
    int retval2;
    BOUSTROPHEDON b1;
    BOUSTROPHEDON b2;
    int b1XSize;
    int b1YSize;
    int b2XSize;
    int b2YSize;
    int b1Size;
    int b2Size;

    b1 = rect;
    b2 = rect;


    retval1 = this.lineGood(b1.xNE++,b1.yNE,b1.xSE++,b1.ySE);
    if(retval1 == 1) {
        //line  good.  keep calling and return the bigger of the two.
        b1.xNE++;
        b1.xSE++;
        b1 = findMaxRectangle(b1);
    }

    retval2 = this.lineGood(b2.xNW,b2.yNW++,b2.xNE,b2.yNE++);
    if(retval2 == 1) {
        //line  good.  keep calling and return the bigger of the two.
        b2.yNE++;
        b2.ySE++;
        b2 = findMaxRectangle(b1);
    }

    if(retval1 == 1 && retval2 == 0){
        //b1 is good, b2 is bad.  Return b1
        return b1;
    }
    else if(retval1 == 0 && retval2 == 1){
        //b2 is good, b1 is bad.  Return b2
        return b2;
    }
    else if(retval1 == 0 && retval2 == 0){
        //neither option worked, return null
        return null;
    }
    else if(retval1 == 1 && retval2 == 1){
        //both options worked, need to return the larger of the two
        b1XSize = b1.xSE - b1.xSW;
        b1YSize = b1.yNW - b1.ySW;
        b2XSize = b2.xSE - b2.xSW;
        b2YSize = b2.yNW - b2.ySW;
        b1Size = b1XSize*b1YSize;
        b2Size = b2XSize*b2YSize;

        if(b1Size > b2Size){
            return b1;
        }
        else{
            return b2;
        }

    }
    //if we get here, something went wrong.
    return null;
}

boolean lineGood(int x1, int y1, int x2, int y2){
    if(x1 == x2){
        //x is the same, see if line on y axis works
        for(int i=0;i<(y2-y1);i++){
            if(this.map.at(y1+i).at(y1).object == true || this.map.at(x1+i).at(y1).boundary == true) {
                //there is an object or boundary in our way
                return false;
            }
        }
    }
    else if(y1 == y2){
        //y is the same, check x axis
        for(int i=0;i<(x2-x1);i++){
            if(this.map.at(x1+i).at(y1).object == true || this.map.at(x1+i).at(y1).boundary == true) {
                //there is an object or boundary in our way
                return false;
            }
        }

    }
    else {
        //something wrong, return null
        return null;
    }
    return true;
}
