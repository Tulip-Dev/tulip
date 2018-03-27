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
#include <deque>
#include <climits>

#include <tulip/tuliphash.h>
#include <tulip/GraphMeasure.h>
#include <tulip/Graph.h>
#include <tulip/GraphParallelTools.h>

using namespace std;
using namespace tlp;
namespace {
inline Iterator<node> *getIt(const Graph *graph, node n, EDGE_TYPE direction) {
  switch (direction) {
  case DIRECTED:
    return graph->getOutNodes(n);

  case INV_DIRECTED:
    return graph->getInNodes(n);

  case UNDIRECTED:
    return graph->getInOutNodes(n);

  default:
    tlp::warning() << __PRETTY_FUNCTION__ << "serious bug..." << std::endl;
    return nullptr;
  }

  return nullptr;
}
}
//================================================================
unsigned int tlp::maxDistance(const Graph *graph, const node n,
                              MutableContainer<unsigned int> &distance, EDGE_TYPE direction) {
  distance.setAll(UINT_MAX);

  tlp::NodeStaticProperty<unsigned int> dist(graph);
  unsigned int result = tlp::maxDistance(graph, graph->nodePos(n), dist, direction);

  for (auto n : graph->nodes())
    distance.set(n, dist[n]);

  return result;
}
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

  while (!fifo.empty()) {
    unsigned int curPos = fifo.front();
    fifo.pop_front();
    unsigned int nDist = distance[curPos] + 1;

    for (auto n : getIt(graph, nodes[curPos], direction)) {
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
// Warning the algorithm is not optimal
double tlp::averagePathLength(const Graph *graph) {
  double result = 0;

  unsigned int nbNodes = graph->numberOfNodes();

  if (nbNodes < 2)
    return result;

  OMP_PARALLEL_MAP_INDICES(nbNodes, [&](unsigned int i) {

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
    OMP_CRITICAL_SECTION(SUMPATH) {
      result += tmp_result;
    }

  });

  result /= (nbNodes * (nbNodes - 1.));
  return result;
}
//================================================================
double tlp::averageClusteringCoefficient(const Graph *graph) {
  tlp::NodeStaticProperty<double> clusters(graph);
  tlp::clusteringCoefficient(graph, clusters, UINT_MAX);
  unsigned int nbNodes = graph->numberOfNodes();
  double sum = 0;

  for (unsigned int i = 0; i < nbNodes; ++i)
    sum += clusters[i];

  return sum / nbNodes;
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
//================================================================
void tlp::reachableNodes(const Graph *graph, const node startNode, std::set<node> &result,
                         unsigned int maxDistance, EDGE_TYPE direction) {
  TLP_HASH_MAP<node, bool> hResult;
  markReachableNodes(graph, startNode, hResult, maxDistance, direction);
  TLP_HASH_MAP<node, bool>::const_iterator itr = hResult.begin();
  TLP_HASH_MAP<node, bool>::const_iterator ite = hResult.end();

  while (itr != ite) {
    result.insert(itr->first);
    ++itr;
  }
}
//================================================================
void tlp::markReachableNodes(const Graph *graph, const node startNode,
                             TLP_HASH_MAP<node, bool> &result, unsigned int maxDistance,
                             EDGE_TYPE direction) {
  deque<node> fifo;
  MutableContainer<bool> visited;
  MutableContainer<unsigned int> distance;
  visited.setAll(false);
  distance.setAll(graph->numberOfNodes());
  fifo.push_back(startNode);
  visited.set(startNode.id, true);
  distance.set(startNode.id, 0);

  while (!fifo.empty()) {
    node current = fifo.front();
    unsigned int curDist = distance.get(current.id);
    fifo.pop_front();

    if (curDist < maxDistance) {
      for (auto itn : getIt(graph, current, direction)) {
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
//=================================================
void tlp::clusteringCoefficient(const Graph *graph, MutableContainer<double> &clusters,
                                unsigned int maxDepth) {
  tlp::NodeStaticProperty<double> vClusters(graph);
  tlp::clusteringCoefficient(graph, vClusters, maxDepth);

  MAP_NODES_AND_INDICES(graph, [&](node n, unsigned int i) {
      clusters.set(n.id, vClusters[i]);
    });
}
//=================================================
void tlp::clusteringCoefficient(const Graph *graph, tlp::NodeStaticProperty<double> &clusters,
                                unsigned int maxDepth) {

  MAP_NODES_AND_INDICES(graph, [&](node n, unsigned int i) {
      TLP_HASH_MAP<node, bool> reachables;
      markReachableNodes(graph, n, reachables, maxDepth);
      double nbEdge = 0; // e(N_v)*2$

      TLP_HASH_MAP<node, bool>::const_iterator itr = reachables.begin();
      TLP_HASH_MAP<node, bool>::const_iterator ite = reachables.end();

      while (itr != ite) {
	node itn = itr->first;

	for (auto e : graph->getInOutEdges(itn)) {
	  const pair<node, node> eEnds = graph->ends(e);

	  if ((reachables.find(eEnds.first) != ite) && (reachables.find(eEnds.second) != ite)) {
	    ++nbEdge;
	  }
	}

	++itr;
      }

      double nNode = reachables.size(); //$|N_v|$

      if (reachables.size() > 1) {
	//$e(N_v)/(\frac{k*(k-1)}{2}}$
	clusters[i] = nbEdge / (nNode * (nNode - 1));
      } else
	clusters[i] = 0;
    });
}
//==================================================
void tlp::dagLevel(const Graph *graph, MutableContainer<unsigned int> &level) {
  tlp::NodeStaticProperty<unsigned int> tmp(graph);
  dagLevel(graph, tmp);
  MAP_NODES_AND_INDICES(graph, [&](node n, unsigned int i) {
      level.set(n.id, tmp[i]);
    });
}

//==================================================
void tlp::dagLevel(const Graph *graph, tlp::NodeStaticProperty<unsigned int> &level) {
  tlp::NodeStaticProperty<unsigned int> totreat(graph);
  deque<node> fifo;
  MAP_NODES_AND_INDICES(graph, [&](node n, unsigned int i) {
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
void tlp::degree(const Graph *graph,
		 tlp::NodeStaticProperty<double> &deg, EDGE_TYPE direction,
                 NumericProperty *weights, bool norm) {
  unsigned int nbNodes = graph->numberOfNodes();

  if (!weights) {
    if (!norm) {
      switch (direction) {
      case UNDIRECTED:
        OMP_PARALLEL_MAP_NODES_AND_INDICES(
            graph, [&](const node n, unsigned int i) { deg[i] = graph->deg(n); });

        break;

      case INV_DIRECTED:
        OMP_PARALLEL_MAP_NODES_AND_INDICES(
            graph, [&](const node n, unsigned int i) { deg[i] = graph->indeg(n); });
        break;

      case DIRECTED:
        OMP_PARALLEL_MAP_NODES_AND_INDICES(
            graph, [&](const node n, unsigned int i) { deg[i] = graph->outdeg(n); });

        break;
      }
    } else {
      double normalization = 1.0;

      if (nbNodes > 1 && graph->numberOfEdges())
        normalization = 1. / (nbNodes - 1);

      switch (direction) {
      case UNDIRECTED:
        OMP_PARALLEL_MAP_NODES_AND_INDICES(
            graph, [&](const node n, unsigned int i) { deg[i] = normalization * graph->deg(n); });
        break;

      case INV_DIRECTED:
        OMP_PARALLEL_MAP_NODES_AND_INDICES(
            graph, [&](const node n, unsigned int i) { deg[i] = normalization * graph->indeg(n); });
        break;

      case DIRECTED:
        OMP_PARALLEL_MAP_NODES_AND_INDICES(graph, [&](const node n, unsigned int i) {
          deg[i] = normalization * graph->outdeg(n);
        });
        break;
      }
    }
  } else {
    if (!norm) {
      switch (direction) {
      case UNDIRECTED:
        OMP_PARALLEL_MAP_NODES_AND_INDICES(graph, [&](const node n, unsigned int i) {
          double nWeight = 0.0;
          for (auto e : graph->getInOutEdges(n)) {
            nWeight += weights->getEdgeDoubleValue(e);
          }
          deg[i] = nWeight;
        });
        break;

      case INV_DIRECTED:
        OMP_PARALLEL_MAP_NODES_AND_INDICES(graph, [&](const node n, unsigned int i) {
          double nWeight = 0.0;
          for (auto e : graph->getInEdges(n)) {
            nWeight += weights->getEdgeDoubleValue(e);
          }
          deg[i] = nWeight;
        });

        break;

      case DIRECTED:
        OMP_PARALLEL_MAP_NODES_AND_INDICES(graph, [&](const node n, unsigned int i) {
          double nWeight = 0.0;
          for (auto e : graph->getInEdges(n)) {
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
        OMP_PARALLEL_MAP_NODES_AND_INDICES(graph, [&](const node n, unsigned int i) {
          double nWeight = 0.0;
          for (auto e : graph->getInOutEdges(n)) {
            nWeight += weights->getEdgeDoubleValue(e);
          }
          deg[i] = nWeight * normalization;
        });

        break;

      case INV_DIRECTED:
        OMP_PARALLEL_MAP_NODES_AND_INDICES(graph, [&](const node n, unsigned int i) {
          double nWeight = 0.0;
          for (auto e : graph->getInEdges(n)) {
            nWeight += weights->getEdgeDoubleValue(e);
          }
          deg[i] = nWeight * normalization;
        });

        break;

      case DIRECTED:
        OMP_PARALLEL_MAP_NODES_AND_INDICES(graph, [&](const node n, unsigned int i) {
          double nWeight = 0.0;
          for (auto e : graph->getInEdges(n)) {
            nWeight += weights->getEdgeDoubleValue(e);
          }
          deg[i] = nWeight * normalization;
        });

        break;
      }
    }
  }
}
