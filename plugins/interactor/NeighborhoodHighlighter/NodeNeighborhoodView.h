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

#include <unordered_map>
#include <tulip/GraphDecorator.h>

namespace tlp {
class DoubleProperty;
}

class NodeNeighborhoodView : public tlp::GraphDecorator {

public:
  enum NeighborNodesType { IN_NEIGHBORS, OUT_NEIGHBORS, IN_OUT_NEIGHBORS };

  NodeNeighborhoodView(tlp::Graph *graph, tlp::node n,
                       NeighborNodesType neighborsNodesType = IN_OUT_NEIGHBORS,
                       unsigned int neighborhoodDist = 1, bool computeReachableSubGraph = false,
                       const std::string &propertyName = "", int nbNodes = 0);

  void updateWithDistance(unsigned int dist);

  bool isElement(tlp::node n) const override;
  bool isElement(tlp::edge e) const override;
  unsigned int nodePos(tlp::node n) const override;
  unsigned int edgePos(tlp::edge e) const override;

  tlp::Iterator<tlp::node> *getNodes() const override;
  tlp::Iterator<tlp::node> *getInNodes(tlp::node n) const override;
  tlp::Iterator<tlp::node> *getOutNodes(tlp::node n) const override;
  tlp::Iterator<tlp::node> *getInOutNodes(tlp::node n) const override;
  tlp::Iterator<tlp::edge> *getEdges() const override;
  tlp::Iterator<tlp::edge> *getOutEdges(tlp::node n) const override;
  tlp::Iterator<tlp::edge> *getInOutEdges(tlp::node n) const override;
  tlp::Iterator<tlp::edge> *getInEdges(tlp::node n) const override;

  const std::vector<tlp::node> &nodes() const override {
    return graphViewNodes;
  }

  unsigned int numberOfNodes() const override {
    return graphViewNodes.size();
  }

  const std::vector<tlp::edge> &edges() const override {
    return graphViewEdges;
  }

  unsigned int numberOfEdges() const override {
    return graphViewEdges.size();
  }

  tlp::Graph *getRoot() const override {
    return const_cast<NodeNeighborhoodView *>(this);
  }

private:
  void getNeighbors(tlp::node n, unsigned int dist, bool noRecursion = false);
  void getInNeighbors(tlp::node n, unsigned int dist, bool noRecursion = false);
  void getOutNeighbors(tlp::node n, unsigned int dist, bool noRecursion = false);

  tlp::node centralNode;

  std::vector<tlp::node> graphViewNodes;
  std::vector<tlp::edge> graphViewEdges;

  std::unordered_map<unsigned int, std::vector<tlp::node>> nodesAtDist;
  std::unordered_map<unsigned int, std::vector<tlp::edge>> edgesAtDist;

  NeighborNodesType neighborsType;
  unsigned int currentDist;
  bool computeReachableSubGraph;
  int nbNodes;
  tlp::DoubleProperty *property;
};

#endif /* REACHABLESUBGRAPHVIEW_H_ */
