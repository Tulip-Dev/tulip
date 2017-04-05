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
#include <deque>
#include <climits>
#ifdef _OPENMP
#include <omp.h>
#endif

#include <tulip/tuliphash.h>
#include <tulip/GraphMeasure.h>
#include <tulip/Graph.h>
#include <tulip/ForEach.h>

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
    return 0;
  }

  return nullptr;
}
} // namespace
//================================================================
unsigned int tlp::maxDistance(const Graph *graph, const node n, MutableContainer<unsigned int> &distance, EDGE_TYPE direction) {
  distance.setAll(UINT_MAX);
  unsigned int nbNodes = graph->numberOfNodes();
  tlp::NodeStaticProperty<unsigned int> dist(graph);
  unsigned int result = tlp::maxDistance(graph, graph->nodePos(n), dist, direction);
  const std::vector<node> &nodes = graph->nodes();
  for (unsigned int i = 0; i < nbNodes; ++i)
    distance.set(nodes[i], dist[i]);
  return result;
}
//================================================================
unsigned int tlp::maxDistance(const Graph *graph, unsigned int nPos, tlp::NodeStaticProperty<unsigned int> &distance, EDGE_TYPE direction) {
  deque<unsigned int> fifo;
  distance.setAll(UINT_MAX);
  fifo.push_back(nPos);
  distance[nPos] = 0;
  const std::vector<node> &nodes = graph->nodes();
  unsigned int maxDist = 0;

  while (!fifo.empty()) {
    unsigned int current = fifo.front();
    fifo.pop_front();
    unsigned int nDist = distance[current] + 1;

    Iterator<node> *itn;
    itn = getIt(graph, nodes[current], direction);
    while (itn->hasNext()) {
      unsigned int n = graph->nodePos(itn->next());
      if (distance[n] == UINT_MAX) {
        fifo.push_back(n);
        distance[n] = nDist;
        maxDist = std::max(maxDist, distance[n] = nDist);
      }
    }

    delete itn;
  }

  return maxDist;
}
//================================================================
// Warning the algorithm is not optimal
double tlp::averagePathLength(const Graph *graph, PluginProgress *pluginProgress) {
  double result = 0;

  size_t nbNodes = graph->numberOfNodes();

  if (nbNodes < 2)
    return result;

  node n;
  int i, steps = 0;
  bool stopfor = false;
#ifdef _OPENMP
#pragma omp parallel for private(i) schedule(dynamic, 1)
#endif
  for (i = 0; i < static_cast<int>(nbNodes); ++i) {
    if (stopfor)
      continue;

// check if we are in the main thread.
// Qt calls can't be done outside of the master thread
#ifdef _OPENMP

    if (omp_get_thread_num() == 0) {
#endif

      if (pluginProgress && ((++steps % 100) == 0)) {
        pluginProgress->progress(steps, nbNodes);

        if (pluginProgress->state() != TLP_CONTINUE) {
#ifdef _OPENMP
#pragma omp critical(STOPFOR)
#endif
          stopfor = true;
        }
      }

#ifdef _OPENMP
    }

#endif
    tlp::NodeStaticProperty<unsigned int> distance(graph);
    maxDistance(graph, i, distance, UNDIRECTED);

    for (int j = 0; j < (int)nbNodes; ++j) {
      if (j == i)
        continue;

      unsigned int d = distance[i];
      if (d != UINT_MAX) {
#ifdef _OPENMP
#pragma omp critical(SUMPATH)
#endif
        result += d;
      }
    }
  }

  if (pluginProgress)
    pluginProgress->progress(nbNodes, nbNodes);

  double nbN = nbNodes;
  result /= (nbN * (nbN - 1.));
  return result;
}
//================================================================
double tlp::averageClusteringCoefficient(const Graph *graph, PluginProgress *pluginProgress) {
  tlp::NodeStaticProperty<double> clusters(graph);
  unsigned int nbNodes = graph->numberOfNodes();
  tlp::clusteringCoefficient(graph, clusters, UINT_MAX, pluginProgress);
  double sum = 0;
  for (unsigned int i = 0; i < clusters.size(); ++i)
    sum += clusters[i];
  return sum / nbNodes;
}
//================================================================
unsigned int tlp::maxDegree(const Graph *graph) {
  unsigned int maxdeg = 0;
  unsigned int nbNodes = graph->numberOfNodes();
  const std::vector<node> &nodes = graph->nodes();
  for (unsigned int i = 0; i < nbNodes; ++i)
    maxdeg = std::max(maxdeg, graph->deg(nodes[i]));

  return maxdeg;
}
//================================================================
unsigned int tlp::minDegree(const Graph *graph) {
  unsigned int nbNodes = graph->numberOfNodes();
  unsigned int mindeg = nbNodes;
  const std::vector<node> &nodes = graph->nodes();
  for (unsigned int i = 0; i < nbNodes; ++i)
    mindeg = std::min(mindeg, graph->deg(nodes[i]));

  return mindeg;
}
//================================================================
void tlp::reachableNodes(const Graph *graph, const node startNode, std::set<node> &result, unsigned int maxDistance, EDGE_TYPE direction) {
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
void tlp::markReachableNodes(const Graph *graph, const node startNode, TLP_HASH_MAP<node, bool> &result, unsigned int maxDistance,
                             EDGE_TYPE direction) {
  deque<node> fifo;
  MutableContainer<bool> visited;
  MutableContainer<unsigned int> distance;
  result.clear();
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
      Iterator<node> *itN = getIt(graph, current, direction);

      while (itN->hasNext()) {
        node itn = itN->next();

        if (!visited.get(itn.id)) {
          fifo.push_back(itn);
          result[itn] = true;
          visited.set(itn.id, true);
          distance.set(itn.id, curDist + 1);
        }
      }

      delete itN;
    }
  }
}
//=================================================
void tlp::clusteringCoefficient(const Graph *graph, MutableContainer<double> &clusters, unsigned int maxDepth, PluginProgress *pp) {
  tlp::NodeStaticProperty<double> vClusters(graph);
  tlp::clusteringCoefficient(graph, vClusters, maxDepth, pp);
  const std::vector<node> &nodes = graph->nodes();
  for (unsigned int i = 0; i < nodes.size(); ++i)
    clusters.set(nodes[i].id, vClusters[i]);
}
//=================================================
void tlp::clusteringCoefficient(const Graph *graph, tlp::NodeStaticProperty<double> &clusters, unsigned int maxDepth, PluginProgress *) {
  const std::vector<node> &nodes = graph->nodes();
  unsigned int nbNodes = nodes.size();

  for (unsigned int i = 0; i < nbNodes; i++) {
    TLP_HASH_MAP<node, bool> reachables;
    markReachableNodes(graph, nodes[i], reachables, maxDepth);
    double nbEdge = 0; // e(N_v)*2$

    TLP_HASH_MAP<node, bool>::const_iterator itr = reachables.begin();
    TLP_HASH_MAP<node, bool>::const_iterator ite = reachables.end();

    while (itr != ite) {
      node itn = itr->first;
      Iterator<edge> *itE = graph->getInOutEdges(itn);

      while (itE->hasNext()) {
        pair<node, node> eEnds = graph->ends(itE->next());

        if ((reachables.find(eEnds.first) != ite) && (reachables.find(eEnds.second) != ite)) {
          ++nbEdge;
        }
      }
      delete itE;
      ++itr;
    }

    double nNode = reachables.size(); //$|N_v|$

    if (reachables.size() > 1) {
      //$e(N_v)/(\frac{k*(k-1)}{2}}$
      clusters[i] = nbEdge / (nNode * (nNode - 1));
    } else
      clusters[i] = 0;
  }
}
//==================================================
void tlp::dagLevel(const Graph *graph, MutableContainer<unsigned int> &level, PluginProgress *pp) {
  tlp::NodeStaticProperty<unsigned int> tmp(graph);
  dagLevel(graph, tmp, pp);
  unsigned int nbNodes = graph->numberOfNodes();
  const std::vector<node> &nodes = graph->nodes();

  for (unsigned int i = 0; i < nbNodes; ++i)
    level.set(nodes[i].id, tmp[i]);
}

//==================================================
void tlp::dagLevel(const Graph *graph, tlp::NodeStaticProperty<unsigned int> &level, PluginProgress *) {
  unsigned int nbNodes = graph->numberOfNodes();
  const vector<node> &nodes = graph->nodes();
  tlp::NodeStaticProperty<unsigned int> totreat(graph);
  deque<node> fifo;
  //===============================================
  for (unsigned int i = 0; i < nbNodes; ++i) {
    node n = nodes[i];
    unsigned int indegree = graph->indeg(n);

    if (indegree == 0) {
      fifo.push_back(n);
      level[i] = 0;
    } else
      totreat[i] = indegree - 1;
  }

  //==============================================
  while (!fifo.empty()) {
    node current = fifo.front();
    fifo.pop_front();
    node child;
    unsigned int curLevel = level.getNodeValue(current) + 1;
    forEach(child, graph->getOutNodes(current)) {
      unsigned int childPos = graph->nodePos(child);
      unsigned int childLevel = totreat[childPos];

      if (childLevel > 0)
        totreat[childPos]= childLevel - 1;
      else {
        level[childPos] = curLevel;
        fifo.push_back(child);
      }
    }
  }
}
