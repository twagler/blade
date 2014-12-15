#ifndef QUADTREE_H
#define QUADTREE_H
#include <time.h>
#include "lawnmap.h"
class QuadTree
{
public:
    QuadTree();
    void build(std::vector<std::vector<LawnCoordinate>>);
};



#endif // QUADTREE_H
