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

#include <algorithm>
#include <climits>
#include "NodeNeighborhoodView.h"

#include <tulip/Iterator.h>
#include <tulip/BooleanProperty.h>
#include <tulip/DoubleProperty.h>

#include "../../utils/PluginNames.h"

template <typename T>
class StlVectorIterator : public tlp::Iterator<T> {

public:
  explicit StlVectorIterator(const std::vector<T> &stlVector) : stlVector(stlVector), curIdx(0) {}

  T next() override {
    return stlVector[curIdx++];
  }

  bool hasNext() override {
    return curIdx < stlVector.size();
  }

private:
  std::vector<T> stlVector;
  unsigned int curIdx;
};

NodeNeighborhoodView::NodeNeighborhoodView(Graph *graph, tlp::node n,
                                           NeighborNodesType neighborsNodesType,
                                           unsigned int neighborhoodDist,
                                           bool computeReachableSubGraph,
                                           const std::string &propertyName, int nbNodes)
    : GraphDecorator(graph), centralNode(n), neighborsType(neighborsNodesType),
      currentDist(neighborhoodDist), computeReachableSubGraph(computeReachableSubGraph),
      nbNodes(nbNodes), property(nullptr) {
  if (!propertyName.empty()) {
    property = graph->getProperty<tlp::DoubleProperty>(propertyName);
  }

  graphViewNodes.push_back(n);

  getNeighbors(n, currentDist);
}

void NodeNeighborhoodView::getNeighbors(tlp::node n, unsigned int dist, bool noRecursion) {

  if (!computeReachableSubGraph) {

    if (neighborsType == IN_NEIGHBORS || neighborsType == IN_OUT_NEIGHBORS) {
      getInNeighbors(n, dist, noRecursion);
    }

    if (neighborsType == OUT_NEIGHBORS || neighborsType == IN_OUT_NEIGHBORS) {
      getOutNeighbors(n, dist, noRecursion);
    }

    if (nbNodes > 0) {
      // filtering nodes
      if (property == nullptr) {
        graphViewNodes.erase(graphViewNodes.begin() + nbNodes + 1, graphViewNodes.end());
      } else {
        std::unordered_map<double, std::vector<tlp::node>> nodesTokeep;
        nodesAtDist[currentDist].clear();

        for (auto n : graphViewNodes) {
          nodesTokeep[property->getNodeValue(n)].push_back(n);
        }

        graphViewNodes.clear();
        graphViewNodes.push_back(n);
        int count = 0;

        for (auto it = nodesTokeep.begin(); it != nodesTokeep.end(); ++it) {
          for (auto n : it->second) {
            if (count++ > nbNodes)
              break;

            graphViewNodes.push_back(n);
            nodesAtDist[currentDist].push_back(n);
          }
        }
      }

      // removing the edges that are connected to filtered nodes
      for (std::vector<tlp::edge>::iterator it = graphViewEdges.begin(); it != graphViewEdges.end();) {
        const std::pair<tlp::node, tlp::node> &eEnds = graph_component->ends(*it);

        if (find(graphViewNodes.begin(), graphViewNodes.end(), eEnds.first) ==
                graphViewNodes.end() ||
            find(graphViewNodes.begin(), graphViewNodes.end(), eEnds.second) ==
                graphViewNodes.end()) {
          it = graphViewEdges.erase(it);
        } else
          ++it;
      }
    }
  } else {
    tlp::BooleanProperty nodesSelection(graph_component);
    nodesSelection.setAllNodeValue(false);
    nodesSelection.setNodeValue(centralNode, true);

    tlp::DataSet dataSet;
    dataSet.set("distance", dist);
    dataSet.set("direction", 2);
    dataSet.set("startingnodes", &nodesSelection);

    tlp::BooleanProperty result(graph_component);
    std::string errorMsg;
    graph_component->applyPropertyAlgorithm(tlp::SelectionAlgorithm::ReachableSubGraphSelection,
                                            &result, errorMsg, &dataSet);

    graphViewNodes.clear();
    graphViewEdges.clear();

    for (auto n2 : graph_component->nodes()) {
      if (result.getNodeValue(n2)) {
        graphViewNodes.push_back(n2);
      }
    }

    for (auto e : graph_component->edges()) {
      if (result.getEdgeValue(e)) {
        graphViewEdges.push_back(e);
      }
    }
  }
}

void NodeNeighborhoodView::getInNeighbors(tlp::node n, unsigned int dist, bool noRecursion) {

  for (auto inNode : graph_component->getInNodes(n)) {
    if (find(graphViewNodes.begin(), graphViewNodes.end(), inNode) == graphViewNodes.end()) {
      graphViewNodes.push_back(inNode);
      nodesAtDist[dist].push_back(inNode);
    }

    tlp::edge e = graph_component->existEdge(inNode, n);

    if (find(graphViewEdges.begin(), graphViewEdges.end(), e) == graphViewEdges.end()) {
      graphViewEdges.push_back(e);
      edgesAtDist[dist].push_back(e);
    }
  }

  if (dist > 1 && !noRecursion) {
    for (auto inNode : graph_component->getInNodes(n)) {
      getInNeighbors(inNode, dist - 1);
    }
  }
}

void NodeNeighborhoodView::getOutNeighbors(tlp::node n, unsigned int dist, bool noRecursion) {

  for (auto outNode : graph_component->getOutNodes(n)) {
    if (find(graphViewNodes.begin(), graphViewNodes.end(), outNode) == graphViewNodes.end()) {
      graphViewNodes.push_back(outNode);
      nodesAtDist[dist].push_back(outNode);
    }

    tlp::edge e = graph_component->existEdge(n, outNode);

    if (find(graphViewEdges.begin(), graphViewEdges.end(), e) == graphViewEdges.end()) {
      graphViewEdges.push_back(e);
      edgesAtDist[dist].push_back(e);
    }
  }

  if (dist > 1 && !noRecursion) {
    for (auto outNode : graph_component->getOutNodes(n)) {
      getOutNeighbors(outNode, dist - 1);
    }
  }
}

void NodeNeighborhoodView::updateWithDistance(const unsigned int dist) {
  if (!computeReachableSubGraph) {
    if (dist > currentDist) {
      if (nodesAtDist.find(dist) == nodesAtDist.end()) {
        for (unsigned int i = 0; i < nodesAtDist[currentDist].size(); ++i) {
          getNeighbors(nodesAtDist[currentDist][i], dist, true);
        }
      } else {
        graphViewNodes.insert(graphViewNodes.end(), nodesAtDist[dist].begin(),
                              nodesAtDist[dist].end());
        graphViewEdges.insert(graphViewEdges.end(), edgesAtDist[dist].begin(),
                              edgesAtDist[dist].end());
      }
    } else if (dist < currentDist) {
      for (unsigned int i = 0; i < nodesAtDist[currentDist].size(); ++i) {
        graphViewNodes.erase(
            remove(graphViewNodes.begin(), graphViewNodes.end(), nodesAtDist[currentDist][i]),
            graphViewNodes.end());
      }

      for (unsigned int i = 0; i < edgesAtDist[currentDist].size(); ++i) {
        graphViewEdges.erase(
            remove(graphViewEdges.begin(), graphViewEdges.end(), edgesAtDist[currentDist][i]),
            graphViewEdges.end());
      }
    }
  } else {
    getNeighbors(centralNode, dist);
  }

  currentDist = dist;
}

bool NodeNeighborhoodView::isElement(const tlp::node n) const {
  return find(graphViewNodes.begin(), graphViewNodes.end(), n) != graphViewNodes.end();
}

unsigned int NodeNeighborhoodView::nodePos(const tlp::node n) const {
  auto nbNodes = graphViewNodes.size();

  for (unsigned int i = 0; i < nbNodes; ++i)
    if (graphViewNodes[i] == n)
      return i;

  return UINT_MAX;
}

bool NodeNeighborhoodView::isElement(const tlp::edge e) const {
  return find(graphViewEdges.begin(), graphViewEdges.end(), e) != graphViewEdges.end();
}

unsigned int NodeNeighborhoodView::edgePos(const tlp::edge e) const {
  auto nbEdges = graphViewEdges.size();

  for (unsigned int i = 0; i < nbEdges; ++i)
    if (graphViewEdges[i] == e)
      return i;

  return UINT_MAX;
}

tlp::Iterator<tlp::node> *NodeNeighborhoodView::getNodes() const {
  return new StlVectorIterator<tlp::node>(graphViewNodes);
}

tlp::Iterator<tlp::node> *NodeNeighborhoodView::getInNodes(const tlp::node n) const {
  std::vector<tlp::node> inNodes;

  for (unsigned int i = 0; i < graphViewEdges.size(); ++i) {
    if (target(graphViewEdges[i]) == n) {
      inNodes.push_back(source(graphViewEdges[i]));
    }
  }

  return new StlVectorIterator<tlp::node>(inNodes);
}

tlp::Iterator<tlp::node> *NodeNeighborhoodView::getOutNodes(const tlp::node n) const {
  std::vector<tlp::node> outNodes;

  for (unsigned int i = 0; i < graphViewEdges.size(); ++i) {
    if (source(graphViewEdges[i]) == n) {
      outNodes.push_back(target(graphViewEdges[i]));
    }
  }

  return new StlVectorIterator<tlp::node>(outNodes);
}

tlp::Iterator<tlp::node> *NodeNeighborhoodView::getInOutNodes(const tlp::node n) const {
  std::vector<tlp::node> inNodes;

  for (unsigned int i = 0; i < graphViewEdges.size(); ++i) {
    if (target(graphViewEdges[i]) == n) {
      inNodes.push_back(source(graphViewEdges[i]));
    }
  }

  std::vector<tlp::node> outNodes;

  for (unsigned int i = 0; i < graphViewEdges.size(); ++i) {
    if (source(graphViewEdges[i]) == n) {
      outNodes.push_back(target(graphViewEdges[i]));
    }
  }

  inNodes.insert(inNodes.end(), outNodes.begin(), outNodes.end());
  return new StlVectorIterator<tlp::node>(inNodes);
}

tlp::Iterator<tlp::edge> *NodeNeighborhoodView::getEdges() const {
  return new StlVectorIterator<tlp::edge>(graphViewEdges);
}

tlp::Iterator<tlp::edge> *NodeNeighborhoodView::getOutEdges(const tlp::node n) const {
  std::vector<tlp::edge> outEdges;

  for (unsigned int i = 0; i < graphViewEdges.size(); ++i) {
    if (source(graphViewEdges[i]) == n) {
      outEdges.push_back(graphViewEdges[i]);
    }
  }

  return new StlVectorIterator<tlp::edge>(outEdges);
}

tlp::Iterator<tlp::edge> *NodeNeighborhoodView::getInOutEdges(const tlp::node n) const {
  std::vector<tlp::edge> inEdges;

  for (unsigned int i = 0; i < graphViewEdges.size(); ++i) {
    if (target(graphViewEdges[i]) == n) {
      inEdges.push_back(graphViewEdges[i]);
    }
  }

  std::vector<tlp::edge> outEdges;

  for (unsigned int i = 0; i < graphViewEdges.size(); ++i) {
    if (source(graphViewEdges[i]) == n) {
      outEdges.push_back(graphViewEdges[i]);
    }
  }

  inEdges.insert(inEdges.end(), outEdges.begin(), outEdges.end());
  return new StlVectorIterator<tlp::edge>(inEdges);
}

tlp::Iterator<tlp::edge> *NodeNeighborhoodView::getInEdges(const tlp::node n) const {
  std::vector<tlp::edge> inEdges;

  for (unsigned int i = 0; i < graphViewEdges.size(); ++i) {
    if (target(graphViewEdges[i]) == n) {
      inEdges.push_back(graphViewEdges[i]);
    }
  }

  return new StlVectorIterator<tlp::edge>(inEdges);
}
