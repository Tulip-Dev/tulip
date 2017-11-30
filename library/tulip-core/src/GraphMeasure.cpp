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
  unsigned int result = tlp::maxDistance(graph, n, dist, direction);

  for (const node &n : graph->nodes())
    distance.set(n, dist[n]);

  return result;
}
//================================================================
unsigned int tlp::maxDistance(const Graph *graph, const node n,
                              tlp::NodeStaticProperty<unsigned int> &distance,
                              EDGE_TYPE direction) {
  deque<node> fifo;
  distance.setAll(UINT_MAX);
  fifo.push_back(n);
  distance[n] = 0;

  unsigned int maxDist = 0;

  while (!fifo.empty()) {
    node current = fifo.front();
    fifo.pop_front();
    unsigned int nDist = distance[current] + 1;

    Iterator<node> *itn;
    itn = getIt(graph, current, direction);

    while (itn->hasNext()) {
      node u = itn->next();
      if (distance[u] == UINT_MAX) {
        fifo.push_back(u);
        distance[u] = nDist;
        maxDist = std::max(maxDist, nDist);
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

  const vector<node> &nodes = graph->nodes();
  size_t nbNodes = graph->numberOfNodes();

  if (nbNodes < 2)
    return result;

  OMP_ITER_TYPE i, steps = 0;
  bool stopfor = false;
#ifdef _OPENMP
#pragma omp parallel for private(i) schedule(dynamic, 1)
#endif
  for (i = 0; i < OMP_ITER_TYPE(nbNodes); ++i) {
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
    maxDistance(graph, nodes[i], distance, UNDIRECTED);

    for (size_t j = 0; j < nbNodes; ++j) {
      if (j == i)
        continue;

      unsigned int d = distance[nodes[j]];

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
  tlp::clusteringCoefficient(graph, clusters, UINT_MAX, pluginProgress);
  double sum = 0;

  for (const node &n : graph->nodes())
    sum += clusters[n];

  return sum / graph->numberOfNodes();
}
//================================================================
unsigned int tlp::maxDegree(const Graph *graph) {
  unsigned int maxdeg = 0;
  for (const node &n : graph->nodes())
    maxdeg = std::max(maxdeg, graph->deg(n));
  return maxdeg;
}
//================================================================
unsigned int tlp::minDegree(const Graph *graph) {
  unsigned int mindeg = graph->numberOfNodes();
  for (const node &n : graph->nodes())
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
void tlp::clusteringCoefficient(const Graph *graph, MutableContainer<double> &clusters,
                                unsigned int maxDepth, PluginProgress *pp) {
  tlp::NodeStaticProperty<double> vClusters(graph);
  tlp::clusteringCoefficient(graph, vClusters, maxDepth, pp);
  for (const node &n : graph->nodes())
    clusters.set(n, vClusters[n]);
}
//=================================================
void tlp::clusteringCoefficient(const Graph *graph, tlp::NodeStaticProperty<double> &clusters,
                                unsigned int maxDepth, PluginProgress *) {

  for (const node &n : graph->nodes()) {
    TLP_HASH_MAP<node, bool> reachables;
    markReachableNodes(graph, n, reachables, maxDepth);
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
      clusters[n] = nbEdge / (nNode * (nNode - 1));
    } else
      clusters[n] = 0;
  }
}
//==================================================
void tlp::dagLevel(const Graph *graph, MutableContainer<unsigned int> &level, PluginProgress *pp) {
  tlp::NodeStaticProperty<unsigned int> tmp(graph);
  dagLevel(graph, tmp, pp);
  for (const node &n : graph->nodes())
    level.set(n, tmp[n]);
}

//==================================================
void tlp::dagLevel(const Graph *graph, tlp::NodeStaticProperty<unsigned int> &level,
                   PluginProgress *) {
  tlp::NodeStaticProperty<unsigned int> totreat(graph);
  deque<node> fifo;

  //===============================================
  for (const node &n : graph->nodes()) {
    unsigned int indegree = graph->indeg(n);

    if (indegree == 0) {
      fifo.push_back(n);
      level[n] = 0;
    } else
      totreat[n] = indegree - 1;
  }

  //==============================================
  while (!fifo.empty()) {
    node current = fifo.front();
    fifo.pop_front();
    node child;
    unsigned int curLevel = level.getNodeValue(current) + 1;
    forEach (child, graph->getOutNodes(current)) {
      unsigned int childLevel = totreat[child];

      if (childLevel > 0)
        totreat[child] = childLevel - 1;
      else {
        level[child] = curLevel;
        fifo.push_back(child);
      }
    }
  }
}

//==================================================
void tlp::degree(const Graph *graph, tlp::NodeStaticProperty<double> &deg, EDGE_TYPE direction,
                 NumericProperty *weights, bool norm) {
  const std::vector<node> &nodes = graph->nodes();
  unsigned int nbNodes = nodes.size();

  if (!weights) {
    if (!norm) {
      switch (direction) {
      case UNDIRECTED:
#ifdef _OPENMP
#pragma omp parallel for
#endif
        for (OMP_ITER_TYPE i = 0; i < OMP_ITER_TYPE(nbNodes); ++i)
          deg[nodes[i]] = graph->deg(nodes[i]);

        break;

      case INV_DIRECTED:
#ifdef _OPENMP
#pragma omp parallel for
#endif
        for (OMP_ITER_TYPE i = 0; i < OMP_ITER_TYPE(nbNodes); ++i)
          deg[nodes[i]] = graph->indeg(nodes[i]);

        break;

      case DIRECTED:
#ifdef _OPENMP
#pragma omp parallel for
#endif
        for (OMP_ITER_TYPE i = 0; i < OMP_ITER_TYPE(nbNodes); ++i)
          deg[nodes[i]] = graph->outdeg(nodes[i]);

        break;
      }
    } else {
      double normalization = 1.0;

      if (nbNodes > 1 && graph->numberOfEdges())
        normalization = 1. / (nbNodes - 1);

      switch (direction) {
      case UNDIRECTED:
#ifdef _OPENMP
#pragma omp parallel for
#endif
        for (OMP_ITER_TYPE i = 0; i < OMP_ITER_TYPE(nbNodes); ++i)
          deg[nodes[i]] = normalization * graph->deg(nodes[i]);

        break;

      case INV_DIRECTED:
#ifdef _OPENMP
#pragma omp parallel for
#endif
        for (OMP_ITER_TYPE i = 0; i < OMP_ITER_TYPE(nbNodes); ++i)
          deg[nodes[i]] = normalization * graph->indeg(nodes[i]);

        break;

      case DIRECTED:
#ifdef _OPENMP
#pragma omp parallel for
#endif
        for (OMP_ITER_TYPE i = 0; i < OMP_ITER_TYPE(nbNodes); ++i)
          deg[nodes[i]] = normalization * graph->outdeg(nodes[i]);

        break;
      }
    }
  } else {
    if (!norm) {
      switch (direction) {
      case UNDIRECTED:
#ifdef _OPENMP
#pragma omp parallel for
#endif
        for (OMP_ITER_TYPE i = 0; i < OMP_ITER_TYPE(nbNodes); ++i) {
          edge e;
          double nWeight = 0.0;
          forEach (e, graph->getInOutEdges(nodes[i])) { nWeight += weights->getEdgeDoubleValue(e); }
          deg[nodes[i]] = nWeight;
        }

        break;

      case INV_DIRECTED:
#ifdef _OPENMP
#pragma omp parallel for
#endif
        for (OMP_ITER_TYPE i = 0; i < OMP_ITER_TYPE(nbNodes); ++i) {
          edge e;
          double nWeight = 0.0;
          forEach (e, graph->getInEdges(nodes[i])) { nWeight += weights->getEdgeDoubleValue(e); }
          deg[nodes[i]] = nWeight;
        }

        break;

      case DIRECTED:
#ifdef _OPENMP
#pragma omp parallel for
#endif
        for (OMP_ITER_TYPE i = 0; i < OMP_ITER_TYPE(nbNodes); ++i) {
          edge e;
          double nWeight = 0.0;
          forEach (e, graph->getInEdges(nodes[i])) { nWeight += weights->getEdgeDoubleValue(e); }
          deg[nodes[i]] = nWeight;
        }

        break;
      }
    } else {
      double normalization = 1.0;
      unsigned int nbEdges = graph->numberOfEdges();

      if (nbNodes > 1 && nbEdges > 0) {
        double sum = 0;

        for (const edge &e : graph->edges())
          sum += fabs(weights->getEdgeDoubleValue(e));

        normalization = (sum / nbEdges) * (nbNodes - 1);

        if (fabs(normalization) < 1E-9)
          normalization = 1.0;
        else
          normalization = 1.0 / normalization;
      }

      switch (direction) {
      case UNDIRECTED:
#ifdef _OPENMP
#pragma omp parallel for
#endif
        for (OMP_ITER_TYPE i = 0; i < OMP_ITER_TYPE(nbNodes); ++i) {
          edge e;
          double nWeight = 0.0;
          forEach (e, graph->getInOutEdges(nodes[i])) { nWeight += weights->getEdgeDoubleValue(e); }
          deg[nodes[i]] = nWeight * normalization;
        }

        break;

      case INV_DIRECTED:
#ifdef _OPENMP
#pragma omp parallel for
#endif
        for (OMP_ITER_TYPE i = 0; i < OMP_ITER_TYPE(nbNodes); ++i) {
          edge e;
          double nWeight = 0.0;
          forEach (e, graph->getInEdges(nodes[i])) { nWeight += weights->getEdgeDoubleValue(e); }
          deg[nodes[i]] = nWeight * normalization;
        }

        break;

      case DIRECTED:
#ifdef _OPENMP
#pragma omp parallel for
#endif
        for (OMP_ITER_TYPE i = 0; i < OMP_ITER_TYPE(nbNodes); ++i) {
          edge e;
          double nWeight = 0.0;
          forEach (e, graph->getInEdges(nodes[i])) { nWeight += weights->getEdgeDoubleValue(e); }
          deg[nodes[i]] = nWeight * normalization;
        }

        break;
      }
    }
  }
}
