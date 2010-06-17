/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
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
#ifndef _TREELEAFLAYOUT_H
#define _TREELEAFLAYOUT_H

#include <tulip/TulipPlugin.h>

/** \addtogroup layout */
/*@{*/
/// TreeLeaf.h - An implementation of a simple tree layout.
/** 
 * This plugin is an implementation of a simple tree layout.
 * All leaves are placed at a distance one (x-coordinates) and the order
 * is the one of a suffix ordering. The y coordinates is the depth in the
 * tree. The other nodes are placed at the center of their children
 * (x-coordinates), and the y-coordinate is their depth in the tree.
 *
 * <b>HISTORY</b>
 *
 * - 01/12/99 Verson 0.0.1: Initial release
 *
 *
 * \note This works on tree. \n
 * Let n be the number of nodes, the algorithm complexity is in O(n).
 *
 *
 * \author David Auber University Bordeaux I France: Email:auber@tulip-software.org
 */
class OrientableLayout;
class OrientableSizeProxy;

class TreeLeaf:public tlp::LayoutAlgorithm { 
public:
  TreeLeaf(const tlp::PropertyContext &);
  ~TreeLeaf();
  bool run();

private:
  float spacing;
  float nodeSpacing;
  std::vector<float> levelHeights;
  float dfsPlacement(tlp::Graph* tree, tlp::node n, float x, float y, unsigned int depth,
		     OrientableLayout *oriLayout, OrientableSizeProxy *oriSize);
  void computeLevelHeights(tlp::Graph* tree, tlp::node n, unsigned int depth,
			   OrientableSizeProxy *oriSize);
};
/*@}*/
#endif

