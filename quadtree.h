#ifndef QUADTREE_H
#define QUADTREE_H
#include <time.h>
#include "lawnmap.h"
class QuadTree
{
public:
    QuadTree* NW;
    QuadTree* NE;
    QuadTree* SW;
    QuadTree* SE;
    QuadTree* parent;
    int quadrant;
    LawnCoordinate leafNW;
    LawnCoordinate leafNE;
    LawnCoordinate leafSW;
    LawnCoordinate leafSE;
    int mowed;
    int boundary;
    int obstacle;
    QuadTree();
    void build(std::vector<std::vector<LawnCoordinate>>);
    std::vector<std::vector<QuadTree>> grow(std::vector<std::vector<QuadTree>>);
};



#endif // QUADTREE_H
