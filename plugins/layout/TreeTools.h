#ifndef TREETOOLS_H
#define TREETOOLS_H

#include <tulip/Node.h>

class SuperGraph;
class Layout;
class Sizes;

node    searchRoot(const SuperGraph* tree);
int     getTreeDepth(const SuperGraph* tree, node root);
bool    isLeaf (const SuperGraph* tree, node n);
float   getNodeX(Layout* pLayout, node current);
float   getNodeY(Layout* pLayout, node current);
float   getNodeHeight(Sizes* size, node current);
float   getNodeWidth(Sizes* size, node current);

#endif

