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
void QuadTree::build(std::vector<std::pair<double,double> > gpsList)
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

    //gpsList is the buffer

    // STEP 2 - get max and min lat and long values


    //set all defaults to first entry in list

    tempX = gpsList[0].first;
    tempY = gpsList[0].second;

    maxX = tempX;
    minX = tempX;
    maxY = tempY;
    minY = tempY;

    for(int it=0;it < (int)gpsList.size(); ++it){
        tempX = gpsList[it].first;
        tempY = gpsList[it].second;
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


    //this 2D vector should be the dimensions of our boundaries
    //LawnCoordinate lawnCoordList[(int)((maxX-minX)*1000000)][(int)((maxY-minY)*1000000)];

    int xSize = (int)((maxX-minX)*1000000);
    int ySize = (int)((maxY-minY)*1000000);

    //vector<LawnCoordinate> lawnCoordList[xSize];
    std::vector< std::vector<LawnCoordinate> > lawnCoordList;
    //vector< vector<LawnCoordinate> > lawnCoordList(xSize, vector<LawnCoordinate>());

    lawnCoordList.resize(xSize);
    for(int j=0;j<xSize;j++){
        lawnCoordList[j].resize(ySize);
    }

    for(int it=0; it < (int)gpsList.size();  ++it){
        tempX = gpsList[it].first;
        tempY = gpsList[it].second;

        lawnCoordList[(int)((maxX-tempX))][(int)(maxY-tempY)].Latitude = tempX;
        lawnCoordList[(int)(maxX-tempX)][(int)(maxY-tempY)].Longitude = tempY;
        lawnCoordList[(int)(maxX-tempX)][(int)(maxY-tempY)].boundary = true;
    }

    //boundaries should now be populated in lawnCoordList object.

    //we can now create the quad tree structure from the leaves

    //STEP 4 - Build quad tree


    //we already have the bottom layer, which is lawnCoordList.  This needs to be turned into the bottom of the quad tree

    std::vector<std::vector<QuadTree> > treeBase;
    treeBase.resize((int)(xSize/2));
    for(int j=0;j<((int)xSize/2);j++){
        treeBase[j].resize(((int)ySize/2));
    }
    for(int j=0;j < (int)lawnCoordList[j].size();j+=2){

        for(int i=0;i < (int)lawnCoordList.size();i+=2){
            //we look at the coord list in blocks of 4 at a time
            (treeBase[i/2][j/2]).leafNW = lawnCoordList[i][j];
            (treeBase[i/2][j/2]).leafNE = lawnCoordList[i+1][j];
            (treeBase[i/2][j/2]).leafSW = lawnCoordList[i][j+1];
            (treeBase[i/2][j/2]).leafSE = lawnCoordList[i+1][j+1];
        }
    }

    xSize = treeBase.size();
    ySize = treeBase[0].size();
    //vector<vector<QuadTree>> currentMapLevel = buildTreeBase(lawnCoordList);

    std::vector<std::vector<QuadTree> > holdingLevel = treeBase;

    while(xSize>2 && ySize>2){
        //when our quad tree is made, we will exit the loop
        //we need to create a new array to hold the current level.  This will need to be purged after every level

        xSize/=2;
        ySize/=2;

        std::vector<std::vector<QuadTree> > currentLevel;
        currentLevel.resize((int)(xSize));

        for(int j=0;j<((int)xSize);j++){
            currentLevel[j].resize(((int)ySize));
        }

        for(int j=0;j < (int)lawnCoordList[j].size();j+=2){

            for(int i=0;i < (int)lawnCoordList.size();i+=2){
            //we look at the coord list in blocks of 4 at a time
            (currentLevel[i/2][j/2]).NW = &holdingLevel[i][j];
            (currentLevel[i/2][j/2]).NE = &holdingLevel[i+1][j];
            (currentLevel[i/2][j/2]).SW = &holdingLevel[i][j+1];
            (currentLevel[i/2][j/2]).SE = &holdingLevel[i+1][j+1];
            holdingLevel[i][j].parent = &currentLevel[i/2][j/2];
            holdingLevel[i+1][j].parent = &currentLevel[i/2][j/2];
            holdingLevel[i][j+1].parent = &currentLevel[i/2][j/2];
            holdingLevel[i+1][j+1].parent = &currentLevel[i/2][j/2];
            }

        }
        holdingLevel = currentLevel;
    }


    //Tree built.  Should have four items left
    this->NW = &holdingLevel[0][0];
    this->NE = &holdingLevel[1][0];
    this->SW = &holdingLevel[0][1];
    this->SE = &holdingLevel[1][1];
    holdingLevel[0][0].parent = this;
    holdingLevel[1][0].parent = this;
    holdingLevel[0][1].parent = this;
    holdingLevel[1][1].parent = this;

}
