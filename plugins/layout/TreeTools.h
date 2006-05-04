#ifndef TREETOOLS_H
#define TREETOOLS_H

#include <tulip/Node.h>

class Graph;
class LayoutProperty;
class SizeProperty;

node    searchRoot(const Graph* tree);
int     getTreeDepth(const Graph* tree, node root);
bool    isLeaf (const Graph* tree, node n);
float   getNodeX(LayoutProperty* pLayout, node current);
float   getNodeY(LayoutProperty* pLayout, node current);
float   getNodeHeight(SizeProperty* size, node current);
float   getNodeWidth(SizeProperty* size, node current);

#endif

