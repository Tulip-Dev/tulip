#ifndef TREETOOLS_H
#define TREETOOLS_H

#include <tulip/Node.h>

namespace tlp {
class Graph;
class LayoutProperty;
class SizeProperty;
}

tlp::node    searchRoot(const tlp::Graph* tree);
int     getTreeDepth(const tlp::Graph* tree, tlp::node root);
bool    isLeaf (const tlp::Graph* tree, tlp::node n);
float   getNodeX(tlp::LayoutProperty* pLayout, tlp::node current);
float   getNodeY(tlp::LayoutProperty* pLayout, tlp::node current);
float   getNodeHeight(tlp::SizeProperty* size, tlp::node current);
float   getNodeWidth(tlp::SizeProperty* size, tlp::node current);

#endif

