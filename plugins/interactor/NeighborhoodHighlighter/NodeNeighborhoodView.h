/**
 *
 * This file is part of Tulip (http://tulip.labri.fr)
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

#include <map>
#include <tulip/GraphDecorator.h>

namespace tlp {
class DoubleProperty;
}

using namespace tlp;

class NodeNeighborhoodView : public GraphDecorator {

public:
  enum NeighborNodesType { IN_NEIGHBORS, OUT_NEIGHBORS, IN_OUT_NEIGHBORS };

  NodeNeighborhoodView(Graph *graph, node n,
                       NeighborNodesType neighborsNodesType = IN_OUT_NEIGHBORS,
                       unsigned int neighborhoodDist = 1, bool computeReachableSubGraph = false,
                       const std::string &propertyName = "", int nbNodes = 0);

  void updateWithDistance(const unsigned int dist);

  bool isElement(const node n) const override;
  bool isElement(const edge e) const override;

  Iterator<node> *getNodes() const override;
  Iterator<node> *getInNodes(const node n) const override;
  Iterator<node> *getOutNodes(const node n) const override;
  Iterator<node> *getInOutNodes(const node n) const override;
  Iterator<edge> *getEdges() const override;
  Iterator<edge> *getOutEdges(const node n) const override;
  Iterator<edge> *getInOutEdges(const node n) const override;
  Iterator<edge> *getInEdges(const node n) const override;

  const std::vector<node> &nodes() const override {
    return graphViewNodes;
  }

  const std::vector<edge> &edges() const override {
    return graphViewEdges;
  }

  Graph *getRoot() const override {
    return const_cast<NodeNeighborhoodView *>(this);
  }

private:
  void getNeighbors(node n, unsigned int dist, bool noRecursion = false);
  void getInNeighbors(node n, unsigned int dist, bool noRecursion = false);
  void getOutNeighbors(node n, unsigned int dist, bool noRecursion = false);

  node centralNode;

  std::vector<node> graphViewNodes;
  std::vector<edge> graphViewEdges;

  std::map<unsigned int, std::vector<node>> nodesAtDist;
  std::map<unsigned int, std::vector<edge>> edgesAtDist;

  NeighborNodesType neighborsType;
  unsigned int currentDist;
  bool computeReachableSubGraph;
  int nbNodes;
  tlp::DoubleProperty *property;
};

#endif /* REACHABLESUBGRAPHVIEW_H_ */
