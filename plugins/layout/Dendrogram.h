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
#ifndef DENDROGRAM_H
#define DENDROGRAM_H

#include <map>
#include <vector>
#include <tulip/TulipPluginHeaders.h>

class OrientableLayout;
class OrientableCoord;
class OrientableSizeProxy;

/** \addtogroup layout */

/** This plugin is an implementation of a dendrogram, an extended implementation
 *  of a "Bio representation" which includes variable orientation
 *  and variable node sizelayout.
 *
 *  \note This works on tree.
 *  Let n be the number of nodes, the algorithm complexity is in O(n).
 *
 *  \author Julien Testut, Antony Durand, Pascal Ollier, Yashvin Nababsing, \n
 *  Sebastien Leclerc, Thibault Ruchon, Eric Dauchier \n
 *  University Bordeaux I France
 **/
class Dendrogram: public tlp::LayoutAlgorithm {
public:
  PLUGININFORMATION("Dendrogram",
                    "Julien Testut, Antony Durand, Pascal Ollier, "
                    "Yashvin Nababsing, Sebastien Leclerc, "
                    "Thibault Ruchon, Eric Dauchier",
                    "03/12/04", "ok", "1.0","Tree")
  Dendrogram(const tlp::PluginContext* context);
  ~Dendrogram();

  bool run();

private:
  float spacing;
  float nodeSpacing;

  std::map<tlp::node, float>   leftshift;
  tlp::node                    root;
  tlp::Graph *tree;
  std::vector<float> levelHeights;

  float   setAllNodesCoordX(tlp::node n, float rightMargin,
                            OrientableLayout *oriLayout,
                            OrientableSizeProxy *oriSize);
  void    setAllNodesCoordY(OrientableLayout *oriLayout,
                            OrientableSizeProxy *oriSize);
  float   computeFatherXPosition(tlp::node father,
                                 OrientableLayout *oriLayout);
  void    shiftAllNodes(tlp::node n, float shift,
                        OrientableLayout *oriLayout);
  void    setNodePosition(tlp::node n, float x, float y, float z,
                          OrientableLayout *oriLayout);
  void    setCoordY(tlp::node n, float& maxYLeaf,
                    OrientableLayout *oriLayout,
                    OrientableSizeProxy *oriSize);
  void computeLevelHeights(tlp::Graph* tree, tlp::node n, unsigned int depth,
                           OrientableSizeProxy *oriSize);
};

#endif



