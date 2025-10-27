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
#include <stack>

#include <tulip/tuliphash.h>
#include <tulip/GraphTools.h>
#include <tulip/GraphMeasure.h>
#ifndef NDEBUG
#include <tulip/AcyclicTest.h>
#include <tulip/ConnectedTest.h>
#endif
#include <tulip/TreeTest.h>
#include <tulip/Graph.h>
#include <tulip/BooleanProperty.h>
#include <tulip/DoubleProperty.h>
#include <tulip/IntegerProperty.h>
#include <tulip/NumericProperty.h>
#include <tulip/MutableContainer.h>
#include <tulip/Ordering.h>
#include <tulip/GraphParallelTools.h>
#include <tulip/Dijkstra.h>

using namespace std;

namespace tlp {
static Iterator<node> *getInNodes(const Graph *graph, const node n) {
  return graph->getInNodes(n);
}
static Iterator<node> *getOutNodes(const Graph *graph, const node n) {
  return graph->getOutNodes(n);
}
static Iterator<node> *getInOutNodes(const Graph *graph, const node n) {
  return graph->getInOutNodes(n);
}

NodesIteratorFn getNodesIterator(EDGE_TYPE direction) {
  switch (direction) {
  case DIRECTED:
    return &getOutNodes;

  case INV_DIRECTED:
    return &getInNodes;

  case UNDIRECTED:
  default:
    return &getInOutNodes;
  }
}

static Iterator<edge> *getInEdges(const Graph *graph, const node n) {
  return graph->getInEdges(n);
}
static Iterator<edge> *getOutEdges(const Graph *graph, const node n) {
  return graph->getOutEdges(n);
}
static Iterator<edge> *getInOutEdges(const Graph *graph, const node n) {
  return graph->getInOutEdges(n);
}

EdgesIteratorFn getEdgesIterator(EDGE_TYPE direction) {
  switch (direction) {
  case DIRECTED:
    return &getOutEdges;

  case INV_DIRECTED:
    return &getInEdges;

  case UNDIRECTED:
  default:
    return &getInOutEdges;
  }
}

//======================================================================
void makeProperDag(Graph *graph, list<node> &addedNodes, tlp_hash_map<edge, edge> &replacedEdges,
                   IntegerProperty *edgeLength) {
  if (TreeTest::isTree(graph))
    return;

  assert(AcyclicTest::isAcyclic(graph));
  // We compute the dag level metric on resulting sg.
  NodeStaticProperty<unsigned int> dLevel(graph);
  dagLevel(graph, dLevel);

  if (edgeLength)
    edgeLength->setAllEdgeValue(1);

  // we now transform the dag in a proper Dag, two linked nodes of a proper dag
  // must have a difference of one of dag level metric.
  const vector<edge> &edges = graph->edges();
  unsigned int nbEdges = edges.size();
  for (unsigned int i = 0; i < nbEdges; ++i) {
    edge e = edges[i];
    auto eEnds = graph->ends(e);
    unsigned int fLevel = dLevel.getNodeValue(eEnds.first);
    unsigned int sLevel = dLevel.getNodeValue(eEnds.second);
    int delta = sLevel - fLevel;

    if (delta > 1) {
      node n1 = graph->addNode();
      replacedEdges[e] = graph->addEdge(eEnds.first, n1);
      addedNodes.push_back(n1);
      dLevel.addNodeValue(n1, fLevel + 1);

      if (delta > 2) {
        node n2 = graph->addNode();
        addedNodes.push_back(n2);
        edge e = graph->addEdge(n1, n2);

        if (edgeLength)
          edgeLength->setEdgeValue(e, delta - 2);

        dLevel.addNodeValue(n2, sLevel - 1);
        n1 = n2;
      }

      graph->addEdge(n1, eEnds.second);
    }
  }

  for (tlp_hash_map<edge, edge>::const_iterator it = replacedEdges.begin();
       it != replacedEdges.end(); ++it)
    graph->delEdge((*it).first);

  assert(AcyclicTest::isAcyclic(graph));
}
//======================================================================
node makeSimpleSource(Graph *graph) {
  assert(AcyclicTest::isAcyclic(graph));

  node startNode = graph->addNode();

  for (auto n : graph->nodes()) {
    if (n != startNode && graph->indeg(n) == 0) {
      graph->addEdge(startNode, n);
    }
  }

  assert(AcyclicTest::isAcyclic(graph));
  return startNode;
}
//======================================================================
vector<vector<node>> computeCanonicalOrdering(PlanarConMap *carte, std::vector<edge> *dummyEdges,
                                              PluginProgress *pluginProgress) {
  Ordering o(carte, pluginProgress, 0, 100, 100); // feedback (0% -> 100%)

  if (dummyEdges != nullptr)
    *dummyEdges = o.getDummyEdges();

  vector<vector<node>> res;
  unsigned int nbMax = o.size();

  if (nbMax) {
    res.reserve(nbMax);

    while (nbMax) {
      res.push_back(o[--nbMax]);
    }
  }

  return res;
}
//======================================================================
std::vector<node> computeGraphCenters(Graph *graph) {
  assert(ConnectedTest::isConnected(graph));
  tlp::NodeStaticProperty<unsigned int> dist(graph);
  const std::vector<node> &nodes = graph->nodes();
  unsigned int nbNodes = nodes.size();
  unsigned int minD = UINT_MAX;
  unsigned int minPos = 0;

  TLP_PARALLEL_MAP_INDICES(nbNodes, [&](unsigned int i) {
    tlp::NodeStaticProperty<unsigned int> tmp(graph);
    unsigned int maxD = tlp::maxDistance(graph, i, tmp, UNDIRECTED);
    dist[i] = maxD;
    TLP_LOCK_SECTION(COMPUTE_MIN) {
      if (minD > maxD) {
        minD = maxD;
        minPos = i;
      }
    }
    TLP_UNLOCK_SECTION(COMPUTE_MIN);
  });

  vector<node> result;

  for (unsigned int i = minPos; i < nbNodes; ++i) {
    if (dist[i] == minD)
      result.push_back(nodes[i]);
  }

  return result;
}
//======================================================================
node graphCenterHeuristic(Graph *graph, PluginProgress *pluginProgress) {
  assert(ConnectedTest::isConnected(graph));

  unsigned int nbNodes = graph->numberOfNodes();

  if (nbNodes == 0)
    return node();

  const vector<node> &nodes = graph->nodes();
  tlp::NodeStaticProperty<bool> toTreat(graph, true);
  tlp::NodeStaticProperty<unsigned int> dist(graph);
  unsigned int n = 0, result = 0;
  unsigned int cDist = UINT_MAX - 2;
  unsigned int nbTry = 2 + sqrt(nbNodes);
  unsigned int maxTries = nbTry;

  while (nbTry) {
    --nbTry;

    if (pluginProgress) {
      pluginProgress->setComment("Computing graph center...");

      if (maxTries - nbTry % 200 == 0)
        pluginProgress->progress(maxTries - nbTry, maxTries);
    }

    if (toTreat[n]) {
      unsigned int di = tlp::maxDistance(graph, n, dist);
      toTreat[n] = false;

      if (di < cDist) {
        result = n;
        cDist = di;
      } else {
        unsigned int delta = di - cDist;

        for (unsigned int v = 0; v < nbNodes; v++)
          if (dist[v] < delta)
            // all the nodes at distance less than delta can't be center
            toTreat[v] = false;
      }

      unsigned int nextMax = 0;

      for (unsigned int v = 0; v < nbNodes; v++) {
        if (dist[v] > (di / 2 + di % 2))
          toTreat[v] = false;
        else {
          if (toTreat[v]) {
            if (dist[v] > nextMax) {
              n = v;
              nextMax = dist[v];
            }
          }
        }
      }

      if (nextMax == 0)
        break;
    }
  }

  if (pluginProgress) {
    pluginProgress->setComment("Graph center computed");
    pluginProgress->progress(100, 100);
  }

  return nodes[result];
}
//======================================================================
void selectSpanningForest(Graph *graph, BooleanProperty *selectionProperty,
                          PluginProgress *pluginProgress) {
  list<node> fifo;

  NodeStaticProperty<bool> nodeFlag(graph);
  const std::vector<node> &nodes = graph->nodes();
  unsigned int nbNodes = nodes.size();

  unsigned int nbSelectedNodes = selectionProperty->numberOfNonDefaultValuatedNodes();

  // get previously selected nodes
  if (nbSelectedNodes) {
    for (unsigned int i = 0; i < nbNodes; ++i) {
      node n = nodes[i];

      if (selectionProperty->getNodeValue(n)) {
        fifo.push_back(n);
        nodeFlag[i] = true;
      }
    }
  } else {
    node n = graph->getOneNode();
    fifo.push_back(n);
    nodeFlag[n] = true;
    nbSelectedNodes = 1;
  }

  EdgeStaticProperty<bool> edgeSel(graph, true);

  // select all nodes
  for (auto n : graph->nodes())
    selectionProperty->setNodeValue(n, true);

  bool ok = true;
  unsigned int edgeCount = 0;

  while (ok) {
    while (!fifo.empty()) {
      node n1 = fifo.front();
      fifo.pop_front();
      for (auto adjit : graph->getOutEdges(n1)) {
        node tgt = graph->target(adjit);
        unsigned int tgtPos = graph->nodePos(tgt);

        if (!nodeFlag[tgtPos]) {
          nodeFlag[tgtPos] = true;
          ++nbSelectedNodes;
          fifo.push_back(tgt);
        } else
          edgeSel[adjit] = false;

        if (pluginProgress) {
          pluginProgress->setComment("Computing a spanning forest...");
          ++edgeCount;

          if (edgeCount == 200) {
            if (pluginProgress->progress(nbSelectedNodes * 100 / graph->numberOfNodes(), 100) !=
                TLP_CONTINUE) {
              return;
            }

            edgeCount = 0;
          }
        }
      }
    }

    ok = false;
    bool degZ = false;
    node goodNode = graph->getOneNode();

    for (unsigned int i = 0; i < nbNodes; ++i) {
      node n = nodes[i];

      if (!nodeFlag[i]) {
        if (!ok) {
          goodNode = n;
          ok = true;
        }

        if (graph->indeg(n) == 0) {
          fifo.push_back(n);
          nodeFlag[i] = true;
          ++nbSelectedNodes;
          degZ = true;
        }

        if (!degZ) {

          if (graph->indeg(n) < graph->indeg(goodNode))
            goodNode = n;
          else {
            if ((graph->indeg(n) == graph->indeg(goodNode)) &&
                (graph->outdeg(n) > graph->outdeg(goodNode)))
              goodNode = n;
          }
        }
      }
    }

    if (ok && (!degZ)) {
      fifo.push_back(goodNode);
      nodeFlag[goodNode] = true;
      ++nbSelectedNodes;
    }
  }

  edgeSel.copyToProperty(selectionProperty);
}
//======================================================================
void selectSpanningTree(Graph *graph, BooleanProperty *selection, PluginProgress *pluginProgress) {
  assert(ConnectedTest::isConnected(graph));
  selection->setAllNodeValue(false);
  selection->setAllEdgeValue(false);

  node root = graphCenterHeuristic(graph, pluginProgress);
  unsigned int size = graph->numberOfNodes();
  unsigned int nbNodes = 1, edgeCount = 0;
  vector<node> roots;
  unsigned int i = 0;
  selection->setNodeValue(root, true);
  roots.push_back(root);

  while (nbNodes != size) {
    root = roots[i];

    for (auto e : graph->getInOutEdges(root)) {

      if (!selection->getEdgeValue(e)) {
        node neighbour = graph->opposite(e, root);

        if (!selection->getNodeValue(neighbour)) {
          selection->setNodeValue(neighbour, true);
          roots.push_back(neighbour);
          nbNodes++;
          selection->setEdgeValue(e, true);

          if (pluginProgress) {
            pluginProgress->setComment("Computing spanning tree...");
            ++edgeCount;

            if (edgeCount % 200 == 0) {
              if (pluginProgress->progress(edgeCount, graph->numberOfEdges()) != TLP_CONTINUE)
                return;
            }
          }
        }
      }
    }

    i++;
  }

  if (pluginProgress) {
    pluginProgress->setComment("Spanning tree computed");
    pluginProgress->progress(100, 100);
  }
}
//======================================================================
struct ltEdge {
  NumericProperty *m;
  ltEdge(NumericProperty *metric) : m(metric) {}
  bool operator()(const edge e1, const edge e2) const {
    return (m->getEdgeDoubleValue(e1) < m->getEdgeDoubleValue(e2));
  }
};

void selectMinimumSpanningTree(Graph *graph, BooleanProperty *selection,
                               NumericProperty *edgeWeight, PluginProgress *pluginProgress) {
  assert(ConnectedTest::isConnected(graph));

  if (!edgeWeight)
    // no weight return a spanning tree
    return selectSpanningTree(graph, selection, pluginProgress);

  const vector<node> &nodes = graph->nodes();

  for (unsigned int i = 0; i < nodes.size(); ++i)
    selection->setNodeValue(nodes[i], true);

  selection->setAllEdgeValue(false);

  NodeStaticProperty<unsigned int> classes(graph);

  unsigned int nbNodes = nodes.size();
  unsigned int numClasses = nbNodes;

  TLP_PARALLEL_MAP_INDICES(nbNodes, [&](unsigned int i) { classes[i] = i; });

  unsigned int maxCount = numClasses;
  unsigned int edgeCount = 0;

  std::vector<edge> sortedEdges(graph->edges());
  std::sort(sortedEdges.begin(), sortedEdges.end(), ltEdge(edgeWeight));
  unsigned int nbEdges = sortedEdges.size();
  unsigned int iE = 0;

  while (numClasses > 1) {
    edge cur;
    unsigned int srcClass = 0, tgtClass = 0;

    for (; iE < nbEdges; ++iE) {
      auto curEnds = graph->ends(cur = sortedEdges[iE]);

      if ((srcClass = classes[curEnds.first]) != (tgtClass = classes[curEnds.second]))
        break;
    }

    selection->setEdgeValue(cur, true);

    if (pluginProgress) {
      pluginProgress->setComment("Computing minimum spanning tree...");
      ++edgeCount;

      if (edgeCount == 200) {
        if (pluginProgress->progress((maxCount - numClasses) * 100 / maxCount, 100) != TLP_CONTINUE)
          return;

        edgeCount = 0;
      }
    }

    TLP_PARALLEL_MAP_INDICES(nbNodes, [&](unsigned int i) {
      if (classes[i] == tgtClass)
        classes[i] = srcClass;
    });

    --numClasses;
  }
}
//======================================================================
struct visitedElt {
  tlp::node n;
  visitedElt *next;

  visitedElt(tlp::node _n) : n(_n), next(nullptr) {}
};

static void bfs(const Graph *graph, node root, std::vector<tlp::node> &nodes,
                MutableContainer<bool> &visited) {
  if (visited.get(root.id))
    return;

  visited.set(root, true);
  visitedElt *first = new visitedElt(root);
  visitedElt *current, *last;
  last = current = first;
  unsigned nbNodes = 1;

  while (current) {
    for (auto neigh : graph->getInOutNodes(current->n)) {
      if (!visited.get(neigh)) {
        visited.set(neigh, true);
        last = last->next = new visitedElt(neigh);
        ++nbNodes;
      }
    }
    current = current->next;
  }

  // add nodes
  nodes.reserve(nbNodes + nodes.size());

  current = first;

  while (current) {
    nodes.push_back(current->n);
    visitedElt *tmp = current->next;
    delete current;
    current = tmp;
  }
}

// bfs from a root node
void bfs(const Graph *graph, node root, std::vector<tlp::node> &nodes) {
  if (graph->isEmpty() == false) {
    if (!root.isValid()) {
      root = graph->getSource();

      if (!root.isValid()) {
        root = graph->getOneNode();
      }
    }

    MutableContainer<bool> visited;
    visited.setAll(false);
    bfs(graph, root, nodes, visited);
  }
}

// cumulative bfs from every node of the graph
void bfs(const Graph *graph, std::vector<tlp::node> &visitedNodes) {
  MutableContainer<bool> visited;
  visited.setAll(false);
  for (const node n : graph->nodes()) {
    bfs(graph, n, visitedNodes, visited);
  }
}
//======================================================================
static void dfs(const Graph *graph, node n, std::vector<node> &nodes,
                MutableContainer<bool> &visited) {
  if (!visited.get(n.id)) {
    visited.set(n.id, true);
    bool edgeOK = (graph == graph->getRoot());
    std::stack<tlp::node> toVisit;
    toVisit.push(n);

    while (!toVisit.empty()) {
      node current = toVisit.top();
      toVisit.pop();
      nodes.push_back(current);
      const std::vector<edge> &edges = graph->allEdges(current);
      unsigned int nbEdges = edges.size();
      unsigned int i = nbEdges;

      while (i) {
        edge e = edges[--i];

        if (edgeOK || graph->isElement(e)) {
          node neigh = graph->opposite(e, current);

          if (!visited.get(neigh.id)) {
            visited.set(neigh.id, true);
            toVisit.push(neigh);
          }
        }
      }
    }
  }
}

// dfs from a root node
void dfs(const Graph *graph, node root, std::vector<node> &visitedNodes) {
  if (graph->isEmpty() == false) {
    if (!root.isValid()) {
      root = graph->getSource();

      if (!root.isValid()) {
        root = graph->getOneNode();
      }
    }

    assert(graph->isElement(root));
    MutableContainer<bool> visited;
    visited.setAll(false);

    dfs(graph, root, visitedNodes, visited);
  }
}

// cumulative dfs from every node of the graph
void dfs(const Graph *graph, std::vector<node> &visitedNodes) {
  MutableContainer<bool> visited;
  visited.setAll(false);
  const std::vector<node> &nodes = graph->nodes();
  unsigned int nbNodes = nodes.size();

  for (unsigned int i = 0; i < nbNodes; ++i) {
    dfs(graph, nodes[i], visitedNodes, visited);
  }
}
//==================================================
void buildNodesUniformQuantification(const Graph *graph, const NumericProperty *prop,
                                     unsigned int k, std::map<double, int> &nodeMapping) {
  // build the histogram of node values
  map<double, int> histogram;
  const std::vector<node> &nodes = graph->nodes();
  unsigned int nbNodes = nodes.size();

  for (unsigned int i = 0; i < nbNodes; ++i) {
    double value = prop->getNodeDoubleValue(nodes[i]);
    map<double, int>::iterator it = histogram.find(value);

    if (it == histogram.end())
      histogram[value] = 1;
    else
      ++(it->second);
  }

  // Build the color map
  double sum = 0;
  double cK = double(nbNodes) / double(k);
  int k2 = 0;
  map<double, int>::iterator it = histogram.begin(), ite = histogram.end();

  for (; it != ite; ++it) {
    sum += it->second;
    nodeMapping[it->first] = k2;

    if (sum > cK * (k2 + 1))
      k2 = ceil(sum / cK) - 1;
  }
}
//==================================================
void buildEdgesUniformQuantification(const Graph *graph, const NumericProperty *prop,
                                     unsigned int k, std::map<double, int> &edgeMapping) {
  // build the histogram of edges values
  map<double, int> histogram;
  for (auto e : graph->edges()) {
    double value = prop->getEdgeDoubleValue(e);
    map<double, int>::iterator it = histogram.find(value);

    if (it == histogram.end())
      histogram[value] = 1;
    else
      ++(it->second);
  }

  // Build the color map
  double sum = 0;
  double cK = double(graph->numberOfEdges()) / double(k);
  int k2 = 0;
  map<double, int>::iterator it = histogram.begin(), ite = histogram.end();

  for (; it != ite; ++it) {
    sum += it->second;
    edgeMapping[it->first] = k2;

    if (sum > cK * (k2 + 1))
      k2 = ceil(sum / cK) - 1;
  }
}

unsigned makeSelectionGraph(const Graph *graph, BooleanProperty *selection, bool *test) {
  Observable::holdObservers();
  unsigned added = 0;
  for (auto e : selection->getEdgesEqualTo(true, graph)) {
    auto ends = graph->ends(e);

    if (!selection->getNodeValue(ends.first)) {
#ifndef NDEBUG
      tlp::debug() << "[Make selection a graph] node #" << ends.first.id << " source of edge #"
                   << e.id << " automatically added to selection.";
#endif
      selection->setNodeValue(ends.first, true);
      added++;

      if (test) {
        *test = false;
        return -1;
      }
    }

    if (!selection->getNodeValue(ends.second)) {
#ifndef NDEBUG
      tlp::debug() << "[Make selection a graph] node #" << ends.second.id << " target of edge #"
                   << e.id << " automatically added to selection.";
#endif
      selection->setNodeValue(ends.second, true);
      added++;

      if (test) {
        *test = false;
        return -1;
      }
    }
  }
  Observable::unholdObservers();

  if (test)
    *test = true;

  return added;
}

#define SMALLEST_WEIGHT 1.E-6

bool selectShortestPaths(const Graph *const graph, node src, node tgt, ShortestPathType pathType,
                         const DoubleProperty *const weights, BooleanProperty *result) {
  EDGE_TYPE direction;

  switch (pathType) {
  case ShortestPathType::OnePath:
  case ShortestPathType::AllPaths:
    direction = UNDIRECTED;
    break;
  case ShortestPathType::OneDirectedPath:
  case ShortestPathType::AllDirectedPaths:
    direction = DIRECTED;
    break;
  case ShortestPathType::OneReversedPath:
  case ShortestPathType::AllReversedPaths:
  default:
    direction = INV_DIRECTED;
  }

  EdgeStaticProperty<double> eWeights(graph, SMALLEST_WEIGHT);
  if (weights) {
    auto fn = [&](edge e, unsigned int i) {
      double val(weights->getEdgeValue(e));
      if (val)
        eWeights[i] = val;
    };
    TLP_PARALLEL_MAP_EDGES_AND_INDICES(graph, fn);
  }

  NodeStaticProperty<double> nodeDistance(graph);
  Dijkstra dijkstra(graph, src, eWeights, nodeDistance, direction);

  if (uint(pathType) < ShortestPathType::AllPaths)
    return dijkstra.searchPath(tgt, result);
  return dijkstra.searchPaths(tgt, result);
}

void markReachableNodes(const Graph *graph, const node startNode, tlp_hash_map<node, bool> &result,
                        unsigned int maxDistance, EDGE_TYPE direction) {
  deque<node> fifo;
  MutableContainer<bool> visited;
  MutableContainer<unsigned int> distance;
  visited.setAll(false);
  distance.setAll(graph->numberOfNodes());
  fifo.push_back(startNode);
  visited.set(startNode.id, true);
  distance.set(startNode.id, 0);

  auto getNodes = getNodesIterator(direction);

  while (!fifo.empty()) {
    node current = fifo.front();
    unsigned int curDist = distance.get(current.id);
    fifo.pop_front();

    if (curDist < maxDistance) {
      for (auto itn : getNodes(graph, current)) {
        if (!visited.get(itn.id)) {
          fifo.push_back(itn);
          result[itn] = true;
          visited.set(itn.id, true);
          distance.set(itn.id, curDist + 1);
        }
      }
    }
  }
}

void computeDijkstra(const Graph *const graph, node src, const EdgeStaticProperty<double> &weights,
                     NodeStaticProperty<double> &nodeDistance, EDGE_TYPE direction,
                     tlp_hash_map<node, std::list<node>> &ancestors, std::stack<node> *queueNodes,
                     MutableContainer<int> *numberOfPaths) {
  Dijkstra dijkstra(graph, src, weights, nodeDistance, direction, queueNodes, numberOfPaths);
  dijkstra.ancestors(ancestors);
}

} // namespace tlp
