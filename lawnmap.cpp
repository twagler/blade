#include "lawnmap.h"
#include <ctime>

LawnMap::LawnMap()
{
    //not used
}

/*LawnMap::LawnMap(vector<LawnCoordinate> vList){
    //this needs to take the initial boundary coordinates and generate a map w/ them
    //grab largest and lowest values for Lat and Lon
    double highLat;
    double highLon;
    double lowLat;
    double lowLon;
    //this is assuming the GPS coordinates will have 7 decimal places of accurancy.  Will probably need to be changed.
    double precision = 10000000;

    LawnCoordinate temp;

    //set initial values to first coordinate
    temp = vList.at(0);
    highLat = temp.Latitude;
    highLon = temp.Longitude;
    lowLat = temp.Latitude;
    lowLon = temp.Longitude;

    //walk through the elements, check if points are higher or lower.  if so, keep track of them
    for(unsigned int i = 0; i < vList.size(); i++)
    {
        temp = vList.at(i);
        if( temp.Latitude > highLat)
        {
            //this coordinate is farther North than our current high Lat value
            //set highLat to this coordinate
            highLat = temp.Latitude;
        }
        else if( temp.Latitude < lowLat){
            //this coordinate is farther South than our current low Lat Value
            //set lowX to this coordinate
            lowLat = temp.Latitude;
        }
        if( temp.Longitude > highLon){
            //this coordinate is farther East than our current high Y Value
            //set highY to this coordinate
            highLon = temp.Longitude;
        }
        else if( temp.Longitude < lowLon){
            //this coordinate is farther south than our current low Y Value
            //set lowY to this coordinate
            lowLon = temp.Longitude;
        }
    }

    //we now have the boundaries of our map and can generate it
    //to change the resolution of the map, we will need to modify the precision

    int LatSize = floor((highLat - lowLat) * precision);
    int LonSize = floor((highLon - lowLon) * precision);

    //so we have a big rectangle made up of the highest/lowest points.  so even if our mowing area isn't a rectangle, our map is.
    //this makes it easier for me to visualize -Alan

    //the map should populate the lat,lon coordinates of that Lawncoordinate
    //upon creation. This needs that logic.
    //Should it be a truncated GPS reading?
    //For instance *example only* :
    //GPS reading => LawnCoordinate
    //37.9516267 => 37.95
    //that way any reading between 37.945 and 37.96 get sent to 37.95

    //i was trying to think of a fast way to do it, using a heap data structure or something similar.... -alan

    this->map.resize(LonSize, vector<LawnCoordinate>(LatSize));


    //this->map ( LonSize, vector<LawnCoordinate> (LatSize, LawnCoordinate()));

    for(unsigned int i = 0; i < vList.size(); i++)
    {
        double lat = vList.at(i).Latitude;
        double lon = vList.at(i).Longitude;

        this->map.at(lat*precision).at(lon*precision).boundary = true;
        //this sets the mowing boundary flag on the (hopefully) appropriate points.
        //we are assuming he vList contains only boundary points.  This might be a bad assumption.
    }
    //map should be generated
}
*/
int LawnMap::boundaryCheck(LawnCoordinate location)
{
        //This should check the map and ensure that the current position is in bounds.
        //get dimensions of map
        int LonSize = this->map.size();
        int LatSize = this->map.at(0).size();

        //If it is in bounds return 1

        //If it is out of bounds return 0
        return 0;  //placeholder
}

int LawnMap::mowedLawnCoordinate(LawnCoordinate currentLocation)
{
    //if our current location is within x cm of any LawnCoordinate
    //mark that coordinate mowed and timestamp it

    //INSERT LOOP OF ALL LAWNCOORDINATES HERE
    //IS THERE A BETTER WAY TO DO THIS?  GOD I HOPE SO...
    //i have no idea how long this will take to run
    //if(gps_distance(currentLocation.LatLon, templocfromloop) <= ARRIVED)
        //currentLocation.mowedTime = time_t.now();
return 0;
}

LawnCoordinate LawnMap::atCoordinate(GPS location)
{
    /*//we need to be able to absorb a range of GPS readings into a single
    //LawnCoordinate to simplify math and logic
    LawnCoordinate tempCoordinate;

    //for each coordinate in map, preferrably a small range of them
    if (gps_distance(location, GPS(tempCoordinate.Latitude,tempCoordinate.Longitude))<=CLOSE_ENOUGH)
        return tempCoordinate;

    //if we get through all of them and don't identify a coordinate to match the GPS reading to
    //return null or something
*/
}

int LawnMap::atWaypoint(LawnCoordinate location)
{
    /*
    //This function compares where we are with where we are going.  If they are the same, return 1.  Otherwise return 0.
    LawnCoordinate to;
    to = this->getNextWaypointCoordinate();

    if (location == to)
       return 1;
    else
        return 0;
        */
}

int LawnMap::canTurn()
{
    /*
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


    */

    return 0; //placeholder
}

void LawnMap::nextRow(){
    //This function sets the new waypoint and places the old waypoint in the fromGPS variables.
}

BOUSTROPHEDON findMaxRectangle(BOUSTROPHEDON rect){
    //need to recursively call with expanded x and y coordinates
/*
    int retval1;
    int retval2;
    BOUSTROPHEDON b1 ,b2
    int b1XSize;
    int b1YSize;
    int b2XSize;
    int b2YSize;
    int b1Size;
    int b2Size;

    b1 = rect;
    b2 = rect;

    retval1 = this.lineGood(b1.NE++,b1.SE++);
    if(retval1 == 1) {
        //line  good.  keep calling and return the bigger of the two.
        b1.NE++;
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
    */
}


/*
 *Trying to redo the lineGood function with LawnCoordinates...
 *
 * boolean lineGood (LawnCoordinate location1, LawnCoordinate location2)
{
    if(location1.Longitude == location2.Longitude)
        for(int i=0;i<(location1.Latitude-location2.Latitude);i++)
            if(this.map.at())

}*/

bool lineGood(int x1, int y1, int x2, int y2)
{
/*    if(x1 == x2){
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
    */
}
