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
#ifndef Tulip_HierarchicalGraph_H
#define Tulip_Hierarchical_H
#include <tulip/tuliphash.h>
#include <tulip/TulipPluginHeaders.h>

class LessThanNode2 {
public:
  tlp::DoubleProperty *metric;
  bool operator() (tlp::node n1, tlp::node n2) {
    return (metric->getNodeValue(n1) < metric->getNodeValue(n2));
  }
};

/** \addtogroup layout */

/**
 * \file
 * \brief An implementation of hierarchical graph drawing algorithm
 *
 * This plugin is an implementation of the hierarchical layout algorithm
 * first published as:
 *
 *  D. Auber, \n
 *  "Tulip - A Huge Graph Visualization Framework", \n
 *  "Book. Graph Drawing Software. (Ed. Michael Junger & Petra Mutzel", \n
 *  "2004", \n
 *  pages 105 - 126.
 *
*/
class HierarchicalGraph:public tlp::LayoutAlgorithm {

public:
  PLUGININFORMATION("Hierarchical Graph","David Auber","23/05/2000","Alpha","1.0","Hierarchical")
  HierarchicalGraph(const tlp::PluginContext* context);
  ~HierarchicalGraph();
  bool run();

private:
  void DagLevelSpanningTree(tlp::Graph* sg, tlp::DoubleProperty* embedding);
  std::vector< std::vector<tlp::node> > grid;
  tlp::DoubleProperty *embedding;
  void twoLayerCrossReduction(tlp::Graph* sg, unsigned int freeLayer, bool);
  void crossReduction(tlp::Graph* sg);
  void computeEdgeBends(const tlp::Graph *mySGraph, tlp::LayoutProperty &tmpLayout,
                        const TLP_HASH_MAP<tlp::edge,tlp::edge> &replacedEdges, const std::vector<tlp::edge> &reversedEdges);
  void computeSelfLoops(tlp::Graph *mySGraph, tlp::LayoutProperty &tmpLayout, std::vector<tlp::SelfLoops> &listSelfLoops);
  void buildGrid(tlp::Graph*);
  unsigned int degree(tlp::Graph* sg, tlp::node n, bool sense);
  void initCross(tlp::Graph* sg, tlp::node n, tlp::MutableContainer< bool >& visited, int id);

  LessThanNode2 lessNode;
  std::string orientation;
  float spacing;
  float nodeSpacing;
};

#endif










