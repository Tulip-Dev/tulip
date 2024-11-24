/**
 *
 * This file is part of Tulip (https://tulip.labri.fr)
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

using namespace std;
using namespace tlp;

template <typename T>
class StlVectorIterator : public Iterator<T> {

public:
  StlVectorIterator(const vector<T> &stlVector) : stlVector(stlVector), curIdx(0) {}

  T next() override {
    return stlVector[curIdx++];
  }

  bool hasNext() override {
    return curIdx < stlVector.size();
  }

private:
  vector<T> stlVector;
  unsigned int curIdx;
};

NodeNeighborhoodView::NodeNeighborhoodView(Graph *graph, node n,
                                           NeighborNodesType neighborsNodesType,
                                           unsigned int neighborhoodDist,
                                           bool computeReachableSubGraph,
                                           const std::string &propertyName, int nbNodes)
    : GraphDecorator(graph), centralNode(n), neighborsType(neighborsNodesType),
      currentDist(neighborhoodDist), computeReachableSubGraph(computeReachableSubGraph),
      nbNodes(nbNodes), property(nullptr) {
  if (!propertyName.empty()) {
    property = graph->getProperty<DoubleProperty>(propertyName);
  }

  graphViewNodes.push_back(n);

  getNeighbors(n, currentDist);
}

void NodeNeighborhoodView::getNeighbors(node n, unsigned int dist, bool noRecursion) {

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
        tlp_hash_map<double, vector<node>> nodesTokeep;
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
      for (vector<edge>::iterator it = graphViewEdges.begin(); it != graphViewEdges.end();) {
        const std::pair<node, node> &eEnds = graph_component->ends(*it);

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
    BooleanProperty nodesSelection(graph_component);
    nodesSelection.setAllNodeValue(false);
    nodesSelection.setNodeValue(centralNode, true);

    DataSet dataSet;
    dataSet.set("distance", dist);
    dataSet.set("direction", 2);
    dataSet.set("startingnodes", &nodesSelection);

    BooleanProperty result(graph_component);
    string errorMsg;
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

void NodeNeighborhoodView::getInNeighbors(node n, unsigned int dist, bool noRecursion) {

  for (auto inNode : graph_component->getInNodes(n)) {
    if (find(graphViewNodes.begin(), graphViewNodes.end(), inNode) == graphViewNodes.end()) {
      graphViewNodes.push_back(inNode);
      nodesAtDist[dist].push_back(inNode);
    }

    edge e = graph_component->existEdge(inNode, n);

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

void NodeNeighborhoodView::getOutNeighbors(node n, unsigned int dist, bool noRecursion) {

  for (auto outNode : graph_component->getOutNodes(n)) {
    if (find(graphViewNodes.begin(), graphViewNodes.end(), outNode) == graphViewNodes.end()) {
      graphViewNodes.push_back(outNode);
      nodesAtDist[dist].push_back(outNode);
    }

    edge e = graph_component->existEdge(n, outNode);

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

bool NodeNeighborhoodView::isElement(const node n) const {
  return find(graphViewNodes.begin(), graphViewNodes.end(), n) != graphViewNodes.end();
}

unsigned int NodeNeighborhoodView::nodePos(const node n) const {
  auto nbNodes = graphViewNodes.size();

  for (unsigned int i = 0; i < nbNodes; ++i)
    if (graphViewNodes[i] == n)
      return i;

  return UINT_MAX;
}

bool NodeNeighborhoodView::isElement(const edge e) const {
  return find(graphViewEdges.begin(), graphViewEdges.end(), e) != graphViewEdges.end();
}

unsigned int NodeNeighborhoodView::edgePos(const edge e) const {
  auto nbEdges = graphViewEdges.size();

  for (unsigned int i = 0; i < nbEdges; ++i)
    if (graphViewEdges[i] == e)
      return i;

  return UINT_MAX;
}

Iterator<node> *NodeNeighborhoodView::getNodes() const {
  return new StlVectorIterator<node>(graphViewNodes);
}

Iterator<node> *NodeNeighborhoodView::getInNodes(const node n) const {
  vector<node> inNodes;

  for (unsigned int i = 0; i < graphViewEdges.size(); ++i) {
    if (target(graphViewEdges[i]) == n) {
      inNodes.push_back(source(graphViewEdges[i]));
    }
  }

  return new StlVectorIterator<node>(inNodes);
}

Iterator<node> *NodeNeighborhoodView::getOutNodes(const node n) const {
  vector<node> outNodes;

  for (unsigned int i = 0; i < graphViewEdges.size(); ++i) {
    if (source(graphViewEdges[i]) == n) {
      outNodes.push_back(target(graphViewEdges[i]));
    }
  }

  return new StlVectorIterator<node>(outNodes);
}

Iterator<node> *NodeNeighborhoodView::getInOutNodes(const node n) const {
  vector<node> inNodes;

  for (unsigned int i = 0; i < graphViewEdges.size(); ++i) {
    if (target(graphViewEdges[i]) == n) {
      inNodes.push_back(source(graphViewEdges[i]));
    }
  }

  vector<node> outNodes;

  for (unsigned int i = 0; i < graphViewEdges.size(); ++i) {
    if (source(graphViewEdges[i]) == n) {
      outNodes.push_back(target(graphViewEdges[i]));
    }
  }

  inNodes.insert(inNodes.end(), outNodes.begin(), outNodes.end());
  return new StlVectorIterator<node>(inNodes);
}

Iterator<edge> *NodeNeighborhoodView::getEdges() const {
  return new StlVectorIterator<edge>(graphViewEdges);
}

Iterator<edge> *NodeNeighborhoodView::getOutEdges(const node n) const {
  vector<edge> outEdges;

  for (unsigned int i = 0; i < graphViewEdges.size(); ++i) {
    if (source(graphViewEdges[i]) == n) {
      outEdges.push_back(graphViewEdges[i]);
    }
  }

  return new StlVectorIterator<edge>(outEdges);
}

Iterator<edge> *NodeNeighborhoodView::getInOutEdges(const node n) const {
  vector<edge> inEdges;

  for (unsigned int i = 0; i < graphViewEdges.size(); ++i) {
    if (target(graphViewEdges[i]) == n) {
      inEdges.push_back(graphViewEdges[i]);
    }
  }

  vector<edge> outEdges;

  for (unsigned int i = 0; i < graphViewEdges.size(); ++i) {
    if (source(graphViewEdges[i]) == n) {
      outEdges.push_back(graphViewEdges[i]);
    }
  }

  inEdges.insert(inEdges.end(), outEdges.begin(), outEdges.end());
  return new StlVectorIterator<edge>(inEdges);
}

Iterator<edge> *NodeNeighborhoodView::getInEdges(const node n) const {
  vector<edge> inEdges;

  for (unsigned int i = 0; i < graphViewEdges.size(); ++i) {
    if (target(graphViewEdges[i]) == n) {
      inEdges.push_back(graphViewEdges[i]);
    }
  }

  return new StlVectorIterator<edge>(inEdges);
}
