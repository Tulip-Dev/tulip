#ifndef TREETOOLS_H
#define TREETOOLS_H

#include <tulip/Node.h>
#include <tulip/Graph.h>
#include <tulip/LayoutProperty.h>
#include <tulip/SizeProperty.h>

//====================================================================
inline bool isLeaf (const tlp::Graph* tree, tlp::node n) {
    return tree->outdeg(n) == 0;
}

//====================================================================
inline float getNodeX(tlp::LayoutProperty* pLayout, tlp::node current) {
    return pLayout->getNodeValue(current).getX();
}

//====================================================================
inline float getNodeY(tlp::LayoutProperty* pLayout, tlp::node current) {
    return pLayout->getNodeValue(current).getY();
}

//====================================================================
inline float getNodeHeight(tlp::SizeProperty* size, tlp::node current) {
    return size->getNodeValue(current).getH();
}

//====================================================================
inline float getNodeWidth(tlp::SizeProperty* size, tlp::node current) {
    return size->getNodeValue(current).getW();
}
#endif

