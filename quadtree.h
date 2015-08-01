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
    //vector<vector<QuadTree> > map;
    int quadrant;
    LawnCoordinate leafNW;
    LawnCoordinate leafNE;
    LawnCoordinate leafSW;
    LawnCoordinate leafSE;
    int mowed;
    int boundary;
    int obstacle;
    QuadTree();
    void build(std::vector<std::pair<double, double> >);
    //QuadTree** buildTree(vector<vector<QuadTree> >);
    //void buildTreeBase(vector<vector<LawnCoordinate> >*);
    //vector<vector<QuadTree>> buildTreeBase(vector<vector<LawnCoordinate>>&);
    //void build(vector<vector<LawnCoordinate>>);
    //QuadTree buildTree(LawnCoordinate[][]);
    //vector<vector<QuadTree>> grow(vector<vector<QuadTree>>);
};



#endif // QUADTREE_H
