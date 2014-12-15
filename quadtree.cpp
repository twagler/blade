#include "quadtree.h"
#include "lawnmap.h"
#include <cstddef>

QuadTree::QuadTree()
{
    //These QuadTree pointrs point to the parents and children of this quad tree
    QuadTree* NW = nullptr;
    QuadTree* NE = nullptr;
    QuadTree* SW = nullptr;
    QuadTree* SE = nullptr;
    QuadTree* parent = nullptr;

    //this int indicates which quadrant of the parent that this quadtree is in
    //0=NW, 1=NE, 2=SW, 3=SE
    int quadrant = -1;


    //if we are at the leaf (bottom) level, these will not populated with valid lawncoordinate objects.  If we are at any level above the bottom level, these are null
    LawnCoordinate leafNW;
    LawnCoordinate leafNE;
    LawnCoordinate leafSW;
    LawnCoordinate leafSE;

    //if the ENTIRE tree is mowed, this is 1.  else 0.
    int mowed = -1;
    //if there is ANY boundary in this tree, this is 1.  else 0
    int boundary = -1;
    //if there is ANY obstacle in this tree, this is 1.  else 0
    int obstacle = -1;
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


        }
    }
 }
