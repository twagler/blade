#include "quadtree.h"
#include "lawnmap.h"
#include <cstddef>

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


    //if we are at the leaf (bottom) level, these will not populated with valid lawncoordinate objects.  If we are at any level above the bottom level, these are null
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
void QuadTree::build(std::vector<std::vector<LawnCoordinate>> vList) {
    //need to assume that vList was created correctly
    int x = 0;
    int y = 0;

    //we build out tree from the bottom up, one quad at a time.  So we look at our vList in 2x2 chunks.
    //x starts at 0 but size starts at 1, plus need to check ahead one since we're grabbing 2 at a time.
    while(x+2<vList.size()){  //while the currext x value +2 is less than the # of x coordinate entries
        while (y+2< vList.at(0).size())  //while the current y value +2 is less than the # of y coordinate entires
        {

        //TODO.  This will involve calling the QuadTree::grow function
        }
    }
 }

std::vector<std::vector<QuadTree>> QuadTree::grow(std::vector<std::vector<QuadTree>> qList) {
    //grows a 2dvector array of QuadTrees into a more QuadTree'd version.
    //Called recursively.  Will eventually end with a qList of lenth 1, with a fully formed QuadTree

    if(qList.size()==1){
        return qList;
    }

    //make the magic happen here
    int newQSizeX = ceil(qList.size()/2);
    int newQSizeY = ceil(qList.at(0).size()/2);

    //make a new 2d QuadTree vector
    //std::vector< std::vector<QuadTree>> newQList( newQSizeX, std::vector<QuadTree>(newQSizeY,0));   //this constructor isn't working... bah.  FIX ME
    std::vector< std::vector<QuadTree>> newQList( newQSizeX, std::vector<QuadTree>());
    for(int n=0;n<newQList.size();n++){
        newQList.at(n).resize(newQSizeY);
    }

    int i=0;
    int j=0;

    for(i = 0; i+2<qList.size(); i=+2){
        for(j = 0; j+2<qList.at(0).size(); j=+2){
            (newQList.at(ceil(i/2))).at(ceil(j/2)).NW = &(qList.at(i)).at(j);
            (newQList.at(ceil(i/2))).at(ceil(j/2)).NE = &(qList.at(i+1)).at(j);
            (newQList.at(ceil(i/2))).at(ceil(j/2)).SW = &(qList.at(i)).at(j+1);
            (newQList.at(ceil(i/2))).at(ceil(j/2)).SE = &(qList.at(i+1)).at(j+1);

        }
        if(j+1<qList.at(0).size() && j+2>=qList.at(0).size()){
            //we have one row left at bottom
            (newQList.at(ceil(i/2))).at(ceil(j/2)).NW = &(qList.at(i)).at(j);
            (newQList.at(ceil(i/2))).at(ceil(j/2)).NE = &(qList.at(i+1)).at(j);
        }
    }
    if(j+1<qList.at(0).size() && j+2>=qList.at(0).size()){
        //we have column left at right
        for(int j = 0; j+2<qList.at(0).size(); j=+2){
            (newQList.at(ceil(i/2))).at(ceil(j/2)).NW = &(qList.at(i)).at(j);
            (newQList.at(ceil(i/2))).at(ceil(j/2)).SW = &(qList.at(i)).at(j+1);

        }
        if(j+1<qList.at(0).size() && j+2>=qList.at(0).size()){
            //we have one item left at bottom
            (newQList.at(ceil(i/2))).at(ceil(j/2)).NW = &(qList.at(i)).at(j);
        }
    }

    //we should now have a QuadTree vector that is more dense and constructed than the last one.

    // continue to recursively call grow
    return grow(newQList);

}
