#ifndef TREETOOLS_H
#define TREETOOLS_H

#include <tulip/Node.h>

class SuperGraph;
class LayoutProxy;
class SizesProxy;

node    searchRoot(const SuperGraph* tree);
int     getTreeDepth(const SuperGraph* tree, node root);
bool    isLeaf (const SuperGraph* tree, node n);
float   getNodeX(LayoutProxy* pLayout, node current);
float   getNodeY(LayoutProxy* pLayout, node current);
float   getNodeHeight(SizesProxy* size, node current);
float   getNodeWidth(SizesProxy* size, node current);

#endif

