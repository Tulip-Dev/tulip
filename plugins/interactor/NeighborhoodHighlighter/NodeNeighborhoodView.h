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

#ifndef REACHABLESUBGRAPHVIEW_H_
#define REACHABLESUBGRAPHVIEW_H_

#include <tulip/GraphDecorator.h>

namespace tlp {
class DoubleProperty;
}

using namespace tlp;

class NodeNeighborhoodView : public GraphDecorator {

public :

  enum NeighborNodesType {IN_NEIGHBORS, OUT_NEIGHBORS, IN_OUT_NEIGHBORS};

  NodeNeighborhoodView(Graph *graph, node n, NeighborNodesType neighborsNodesType = IN_OUT_NEIGHBORS,
                       unsigned int neighborhoodDist = 1,
                       bool computeReachableSubGraph = false,
                       const std::string& propertyName = "", int nbNodes = 0);

  void updateWithDistance(const unsigned int dist);

  bool isElement(const node n) const;
  bool isElement(const edge e) const;

  Iterator<node>* getNodes() const;
  Iterator<node>* getInNodes(const node n) const;
  Iterator<node>* getOutNodes(const node n) const;
  Iterator<node>* getInOutNodes(const node n) const;
  Iterator<edge>* getEdges() const;
  Iterator<edge>* getOutEdges(const node n) const;
  Iterator<edge>* getInOutEdges(const node n) const;
  Iterator<edge>* getInEdges(const node n) const;

  Graph *getRoot() const {
    return const_cast<NodeNeighborhoodView*>(this);
  }

private :

  void getNeighbors(node n, unsigned int dist, bool noRecursion = false);
  void getInNeighbors(node n, unsigned int dist, bool noRecursion = false);
  void getOutNeighbors(node n, unsigned int dist, bool noRecursion = false);

  node centralNode;

  std::vector<node> graphViewNodes;
  std::vector<edge> graphViewEdges;

  std::map<unsigned int, std::vector<node> > nodesAtDist;
  std::map<unsigned int, std::vector<edge> > edgesAtDist;

  NeighborNodesType neighborsType;
  unsigned int currentDist;
  bool computeReachableSubGraph;
  int nbNodes;
  tlp::DoubleProperty* property;
};


#endif /* REACHABLESUBGRAPHVIEW_H_ */
