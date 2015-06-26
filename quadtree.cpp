#include "quadtree.h"
#include <cstddef>
#include "math.h"

QuadTree::QuadTree()
{
    //These QuadTree pointrs point to the parents and children of this quad tree
    this->NW = nullptr;
    this->NE = nullptr;
    this->SW = nullptr;
    this->SE = nullptr;
    this->parent = nullptr;

    //this int indicates which quadrant of the parent that this quadtree is in
    //0=NW, 1=NE, 2=SW, 3=SE
    this->quadrant = -1;


    //if we are at the leaf (bottom) level, these will be populated with valid lawncoordinate objects.
    //If we are at any level above the bottom level, these are null
    //LawnCoordinate leafNW;
    //LawnCoordinate leafNE;
    //LawnCoordinate leafSW;
    //LawnCoordinate leafSE;

    //if the ENTIRE tree is mowed, this is 1.  else 0.
    this->mowed = -1;
    //if there is ANY boundary in this tree, this is 1.  else 0
    this->boundary = -1;
    //if there is ANY obstacle in this tree, this is 1.  else 0
    this->obstacle = -1;
}
/*
 * builds the quadtree
 */
void QuadTree::build(vector<double[2]> gpsList)
{

    /*    This consists of 4 steps:
     *
     * 	1.  Accept in buffer (vector of arrays) of GPS Data
     *
     *  2.	Get min and max values for x and y from vector of data
     *
     * 	3.	Build bottom level of quad tree (leaves)
     *
     *  4. 	construct quad tree
     *
     *	5.	Correct map errors
     */


    // STEP 1 - accept buffer of GPS data

    //need to assume that  gpsList was created correctly
    //assume only first 6 decimal digits are displayed
    double maxX = 0;
    double maxY = 0;
    double minX = 0;
    double minY = 0;
    double tempX = 0;
    double tempY = 0;

    // STEP 2 - get max and min lat and long values


    //set all defaults to first entry in list
    tempX = gpsList.at(0)[0];
    tempY = gpsList.at(0)[1];

    maxX = tempX;
    minX = tempX;
    maxY = tempY;
    minY = tempY;

    for(int it=0;it < gpsList.size(); ++it){
        tempX = gpsList.at(it)[0];
        tempY = gpsList.at(it)[1];
        if( tempX > maxX) {
            maxX = tempX;
        }
        else if( tempX < minX){
            minX = tempX;
        }

        if( tempY > maxY) {
            maxY = tempY;
        }
        else if( tempY < minY){
            minY = tempY;
        }
    }
    //we shouhld now have the max and min GPS coordinates for lat and long

    //STEP 3 - Build leaf bottom of quad tree framework

    /*the quad tree is mostly empty structure. only the boundaries will be filled in at this point.
     * Eventually objects will be populated and the quad tree will be used to track mowed vs nonmowed sections.
     *
     * The size of the bottom layer of the quad tree should be equal to the MaxX-MinX by MaxY-MinY
     */


    //this 2D array should be the dimensions of our boundaries
    LawnCoordinate lawnCoordList[(int)((maxX-minX)*1000000)][(int)((maxY-minY)*1000000)];

    for(int it=0; it < gpsList.size();  ++it){
        tempX = gpsList.at(it)[0];
        tempY = gpsList.at(it)[1];

        lawnCoordList[(int)((maxX-tempX))][(int)(maxY-tempY)].Latitude = tempX;
        lawnCoordList[(int)(maxX-tempX)][(int)(maxY-tempY)].Longitude = tempY;
        lawnCoordList[(int)(maxX-tempX)][(int)(maxY-tempY)].boundary = true;
    }

    //boundaries should now be populated in lawnCoordList object.

    //we can now create the quad tree structure from the leaves

    //STEP 4 - Build quad tree

    this->map = *(this->buildTree(*lawnCoordList));


}
QuadTree* QuadTree::buildTree(LawnCoordinate *lCList){
}

