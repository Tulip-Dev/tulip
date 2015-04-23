/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
 *
 * Tulip is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Tulip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */
#ifndef TREETOOLS_H
#define TREETOOLS_H

#include <tulip/Node.h>
#include <tulip/LayoutProperty.h>
#include <tulip/SizeProperty.h>

namespace tlp {
class Graph;
}

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

