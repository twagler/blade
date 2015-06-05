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
/*void QuadTree::build(vector<double[2]> gpsList)
{

        This consists of 4 steps:
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
     *


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
    tempX = gpsList.at(gpsList.begin())[0];
    tempY = gpsList.at(gpsList.begin())[1];

    maxX = tempX;
    minX = tempX;
    maxY = tempY;
    minY = tempY;

    for(vector<double[2]>::iterator it = gpsList.begin(); it != gpsList.end(); ++it){
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

    the quad tree is mostly empty structure. only the boundaries will be filled in at this point.
     * Eventually objects will be populated and the quad tree will be used to track mowed vs nonmowed sections.
     *
     * The size of the bottom layer of the quad tree should be equal to the MaxX-MinX by MaxY-MinY
     *



    LawnCoordinate lawnCoordList[(maxX-minX)*1000000][(maxY-minY)*1000000]; //this 2D array should be the dimensions of our boundaries

    for(vector<double[2]>::iterator it = gpsList.begin(); it != gpsList.end(); ++it){
        tempX = gpsList.at(it)[0];
        tempY = gpsList.at(it)[1];

        lawnCoordList[maxX-tempX][maxY-tempY].Latitude = tempX;
        lawnCoordList[maxX-tempX][maxY-tempY].Longitude = tempY;
        lawnCoordList[maxX-tempX][maxY-tempY].boundary = true;
    }

    //boundaries should now be populated in lawnCoordList object.

    //we can now create the quad tree structure from the leaves

    //STEP 4 - Build quad tree

    this->map = this->buildTree(lawnCoordList);

}

QuadTree QuadTree::buildTree(LawnCoordinate lCList[][]){
}

 i'm pretty sure the following should be deleted, but i'm leaving for now.
 * -Alan 20150505
 *
 *
vector<vector<QuadTree>> QuadTree::grow(vector<vector<QuadTree>> qList)
{
    //grows a 2dvector array of QuadTrees into a more QuadTree'd version.
    //Called recursively.  Will eventually end with a qList of lenth 1, with a fully formed QuadTree

    if(qList.size()==1)
    {
        return qList;
    }

    //make the magic happen here
    int newQSizeX = ceil(qList.size()/2);
    int newQSizeY = ceil(qList.at(0).size()/2);

    //make a new 2d QuadTree vector
    //vector< vector<QuadTree>> newQList( newQSizeX, vector<QuadTree>(newQSizeY,0));   //this constructor isn't working... bah.  FIX ME
    vector< vector<QuadTree>> newQList( newQSizeX, vector<QuadTree>());
    for(unsigned int n=0;n<newQList.size();n++)
    {
        newQList.at(n).resize(newQSizeY);
    }

    unsigned int i=0;
    unsigned int j=0;

    for(i = 0; i+2<qList.size(); i=+2)
    {
        for(j = 0; j+2<qList.at(0).size(); j=+2)
        {
            (newQList.at(ceil(i/2))).at(ceil(j/2)).NW = &(qList.at(i)).at(j);
            (newQList.at(ceil(i/2))).at(ceil(j/2)).NE = &(qList.at(i+1)).at(j);
            (newQList.at(ceil(i/2))).at(ceil(j/2)).SW = &(qList.at(i)).at(j+1);
            (newQList.at(ceil(i/2))).at(ceil(j/2)).SE = &(qList.at(i+1)).at(j+1);

        }
        if(j+1<qList.at(0).size() && j+2>=qList.at(0).size())
        {
            //we have one row left at bottom
            (newQList.at(ceil(i/2))).at(ceil(j/2)).NW = &(qList.at(i)).at(j);
            (newQList.at(ceil(i/2))).at(ceil(j/2)).NE = &(qList.at(i+1)).at(j);
        }
    }
    if(j+1<qList.at(0).size() && j+2>=qList.at(0).size())
    {
        //we have column left at right
        for(unsigned int j = 0; j+2<qList.at(0).size(); j=+2)
        {
            (newQList.at(ceil(i/2))).at(ceil(j/2)).NW = &(qList.at(i)).at(j);
            (newQList.at(ceil(i/2))).at(ceil(j/2)).SW = &(qList.at(i)).at(j+1);

        }
        if(j+1<qList.at(0).size() && j+2>=qList.at(0).size())
        {
            //we have one item left at bottom
            (newQList.at(ceil(i/2))).at(ceil(j/2)).NW = &(qList.at(i)).at(j);
        }
    }

    //we should now have a QuadTree vector that is more dense and constructed than the last one.

    // continue to recursively call grow
    return grow(newQList);

}
*/
