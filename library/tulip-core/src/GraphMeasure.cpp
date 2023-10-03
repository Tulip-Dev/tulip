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
#include <deque>
#include <stack>

#include <unordered_set>
#include <tulip/GraphMeasure.h>
#include <tulip/Graph.h>
#include <tulip/GraphParallelTools.h>
#include <tulip/Dijkstra.h>

using namespace std;
using namespace tlp;

//================================================================
unsigned int tlp::maxDistance(const Graph *graph, unsigned int nPos,
                              tlp::NodeStaticProperty<unsigned int> &distance,
                              EDGE_TYPE direction) {
  deque<unsigned int> fifo;
  distance.setAll(UINT_MAX);
  fifo.push_back(nPos);
  distance[nPos] = 0;
  const std::vector<node> &nodes = graph->nodes();
  unsigned int maxDist = 0;
  auto getNodes = getNodesIterator(direction);

  while (!fifo.empty()) {
    unsigned int curPos = fifo.front();
    fifo.pop_front();
    unsigned int nDist = distance[curPos] + 1;

    for (auto n : getNodes(graph, nodes[curPos])) {
      nPos = graph->nodePos(n);
      if (distance[nPos] == UINT_MAX) {
        fifo.push_back(nPos);
        distance[nPos] = nDist;
        maxDist = std::max(maxDist, nDist);
      }
    }
  }

  return maxDist;
}
//================================================================
double tlp::maxDistance(const Graph *graph, const unsigned int nPos,
                        tlp::NodeStaticProperty<double> &distance,
                        const NumericProperty *const weights, EDGE_TYPE direction) {
  if (!weights) {
    NodeStaticProperty<unsigned int> dist_int(graph, 0);
    unsigned int res = maxDistance(graph, nPos, dist_int, direction);
    for (auto n : graph->getNodes()) {
      distance[n] = double(dist_int[n]);
    }
    return double(res);
  }

  EdgeStaticProperty<double> eWeights(graph);
  eWeights.copyFromNumericProperty(weights);

  std::stack<node> queueNode;
  MutableContainer<int> nb_paths;
  Dijkstra dijkstra(graph, graph->nodes()[nPos], eWeights, distance, direction, &queueNode,
                    &nb_paths);
  // compute max distance from graph->nodes()[nPos]
  // by taking first reachable node in the queue
  while (!queueNode.empty()) {
    node n = queueNode.top();
    queueNode.pop();
    if (nb_paths.get(n.id) > 0)
      return distance[n];
  }
  return 0.;
}
//================================================================
// Warning the algorithm is not optimal
double tlp::averagePathLength(const Graph *graph) {
  double result = 0;

  unsigned int nbNodes = graph->numberOfNodes();

  if (nbNodes < 2)
    return result;

  TLP_PARALLEL_MAP_INDICES(nbNodes, [&](unsigned int i) {
    tlp::NodeStaticProperty<unsigned int> distance(graph);
    maxDistance(graph, i, distance, UNDIRECTED);

    double tmp_result = 0;

    for (unsigned int j = 0; j < nbNodes; ++j) {
      if (j == i)
        continue;

      unsigned int d = distance[j];
      if (d != UINT_MAX) {
        tmp_result += d;
      }
    }
    TLP_LOCK_SECTION(SUMPATH) {
      result += tmp_result;
    }
    TLP_UNLOCK_SECTION(SUMPATH);
  });

  result /= (nbNodes * (nbNodes - 1.));
  return result;
}
//================================================================
double tlp::averageClusteringCoefficient(const Graph *graph) {
  tlp::NodeStaticProperty<double> clusters(graph);
  tlp::clusteringCoefficient(graph, clusters);

  double sum = 0;
  for (auto v : clusters) {
    sum += v;
  }

  return sum / graph->numberOfNodes();
}
//================================================================
unsigned int tlp::maxDegree(const Graph *graph) {
  unsigned int maxdeg = 0;
  for (auto n : graph->nodes())
    maxdeg = std::max(maxdeg, graph->deg(n));
  return maxdeg;
}
//================================================================
unsigned int tlp::minDegree(const Graph *graph) {
  unsigned int mindeg = graph->numberOfNodes();
  for (auto n : graph->nodes())
    mindeg = std::min(mindeg, graph->deg(n));
  return mindeg;
}
//=================================================
void tlp::clusteringCoefficient(const Graph *graph, tlp::NodeStaticProperty<double> &clusters,
                                unsigned int) {

  TLP_MAP_NODES_AND_INDICES(graph, [&](node n, unsigned int i) {
    vector<node> reachables;
    for (node nei : graph->getInOutNodes(n)) {
      reachables.push_back(nei);
    }

    unsigned nbEdgesN(0);
    auto itr = reachables.end();
    for (auto i = reachables.begin(); i != itr - 1; ++i) {
      for (auto j = i + 1; j != itr; ++j) {
        if (graph->hasEdge(*i, *j, false)) {
          ++nbEdgesN;
        }
      }
    }

    double nbEdge(graph->deg(n) + nbEdgesN);
    double nNode = reachables.size() + 1;
    clusters[i] = nbEdge / ((nNode * (nNode - 1)) / 2);
  });
}
//==================================================
void tlp::dagLevel(const Graph *graph, tlp::NodeStaticProperty<unsigned int> &level) {
  tlp::NodeStaticProperty<unsigned int> totreat(graph);
  deque<node> fifo;
  TLP_MAP_NODES_AND_INDICES(graph, [&](node n, unsigned int i) {
    unsigned int indegree = graph->indeg(n);

    if (indegree == 0) {
      fifo.push_back(n);
      level[i] = 0;
    } else
      totreat[i] = indegree - 1;
  });

  //==============================================
  while (!fifo.empty()) {
    node current = fifo.front();
    fifo.pop_front();
    unsigned int curLevel = level.getNodeValue(current) + 1;
    for (auto child : graph->getOutNodes(current)) {
      unsigned int childPos = graph->nodePos(child);
      unsigned int childLevel = totreat[childPos];

      if (childLevel > 0)
        totreat[childPos] = childLevel - 1;
      else {
        level[childPos] = curLevel;
        fifo.push_back(child);
      }
    }
  }
}

//==================================================
void tlp::degree(const Graph *graph, tlp::NodeStaticProperty<double> &deg, EDGE_TYPE direction,
                 NumericProperty *weights, bool norm) {
  unsigned int nbNodes = graph->numberOfNodes();

  if (!weights) {
    if (!norm) {
      switch (direction) {
      case UNDIRECTED:
        TLP_PARALLEL_MAP_NODES_AND_INDICES(
            graph, [&](const node n, unsigned int i) { deg[i] = graph->deg(n); });

        break;

      case INV_DIRECTED:
        TLP_PARALLEL_MAP_NODES_AND_INDICES(
            graph, [&](const node n, unsigned int i) { deg[i] = graph->indeg(n); });
        break;

      case DIRECTED:
        TLP_PARALLEL_MAP_NODES_AND_INDICES(
            graph, [&](const node n, unsigned int i) { deg[i] = graph->outdeg(n); });

        break;
      }
    } else {
      double normalization = 1.0;

      if (nbNodes > 1 && graph->numberOfEdges())
        normalization = 1. / (nbNodes - 1);

      switch (direction) {
      case UNDIRECTED:
        TLP_PARALLEL_MAP_NODES_AND_INDICES(
            graph, [&](const node n, unsigned int i) { deg[i] = normalization * graph->deg(n); });
        break;

      case INV_DIRECTED:
        TLP_PARALLEL_MAP_NODES_AND_INDICES(
            graph, [&](const node n, unsigned int i) { deg[i] = normalization * graph->indeg(n); });
        break;

      case DIRECTED:
        TLP_PARALLEL_MAP_NODES_AND_INDICES(graph, [&](const node n, unsigned int i) {
          deg[i] = normalization * graph->outdeg(n);
        });
        break;
      }
    }
  } else {
    if (!norm) {
      switch (direction) {
      case UNDIRECTED:
        TLP_PARALLEL_MAP_NODES_AND_INDICES(graph, [&](const node n, unsigned int i) {
          double nWeight = 0.0;
          for (auto e : graph->getInOutEdges(n)) {
            nWeight += weights->getEdgeDoubleValue(e);
          }
          deg[i] = nWeight;
        });
        break;

      case INV_DIRECTED:
        TLP_PARALLEL_MAP_NODES_AND_INDICES(graph, [&](const node n, unsigned int i) {
          double nWeight = 0.0;
          for (auto e : graph->getInEdges(n)) {
            nWeight += weights->getEdgeDoubleValue(e);
          }
          deg[i] = nWeight;
        });

        break;

      case DIRECTED:
        TLP_PARALLEL_MAP_NODES_AND_INDICES(graph, [&](const node n, unsigned int i) {
          double nWeight = 0.0;
          for (auto e : graph->getOutEdges(n)) {
            nWeight += weights->getEdgeDoubleValue(e);
          }
          deg[i] = nWeight;
        });

        break;
      }
    } else {
      double normalization = 1.0;
      unsigned int nbEdges = graph->numberOfEdges();

      if (nbNodes > 1 && nbEdges > 0) {
        double sum = 0;

        for (auto e : graph->edges())
          sum += fabs(weights->getEdgeDoubleValue(e));

        normalization = (sum / nbEdges) * (nbNodes - 1);

        if (fabs(normalization) < 1E-9)
          normalization = 1.0;
        else
          normalization = 1.0 / normalization;
      }

      switch (direction) {
      case UNDIRECTED:
        TLP_PARALLEL_MAP_NODES_AND_INDICES(graph, [&](const node n, unsigned int i) {
          double nWeight = 0.0;
          for (auto e : graph->getInOutEdges(n)) {
            nWeight += weights->getEdgeDoubleValue(e);
          }
          deg[i] = nWeight * normalization;
        });

        break;

      case INV_DIRECTED:
        TLP_PARALLEL_MAP_NODES_AND_INDICES(graph, [&](const node n, unsigned int i) {
          double nWeight = 0.0;
          for (auto e : graph->getInEdges(n)) {
            nWeight += weights->getEdgeDoubleValue(e);
          }
          deg[i] = nWeight * normalization;
        });

        break;

      case DIRECTED:
        TLP_PARALLEL_MAP_NODES_AND_INDICES(graph, [&](const node n, unsigned int i) {
          double nWeight = 0.0;
          for (auto e : graph->getOutEdges(n)) {
            nWeight += weights->getEdgeDoubleValue(e);
          }
          deg[i] = nWeight * normalization;
        });

        break;
      }
    }
  }
}
