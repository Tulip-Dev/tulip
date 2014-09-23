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

#include <tulip/GraphMeasure.h>
#include <tulip/Graph.h>
#include <tulip/ForEach.h>

using namespace std;
using namespace tlp;
namespace {
inline Iterator<node> *getIt(const Graph *graph, node n, EDGE_TYPE direction) {
  switch(direction) {
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

  return NULL;
}
}
//================================================================
unsigned int tlp::maxDistance(const Graph *graph, const node n,
                              MutableContainer<unsigned int> &distance,
                              EDGE_TYPE direction) {
  deque<node> fifo;
  distance.setAll(UINT_MAX);
  fifo.push_back(n);
  distance.set(n.id, 0);
  unsigned int maxDist = 0;

  while(!fifo.empty()) {
    node current = fifo.front();
    fifo.pop_front();
    unsigned int nDist = distance.get(current.id) + 1;
    Iterator<node>* itn;
    itn = getIt(graph, current, direction);

    while (itn->hasNext()) {
      node n = itn->next();

      if (distance.get(n.id) == UINT_MAX) {
        fifo.push_back(n);
        distance.set(n.id, nDist);
        maxDist = std::max(maxDist, nDist);
      }
    }

    delete itn;
  }

  return maxDist;
}
//================================================================
//Warning the algorithm is not optimal
double tlp::averagePathLength(const Graph *graph,
                              PluginProgress *pluginProgress) {
  double result = 0;

  size_t nbNodes = graph->numberOfNodes();

  if (nbNodes < 2) return result;

  node n;
  int steps = 0;
  vector<node> nodes(nbNodes);
  int i = 0;
  forEach(n, graph->getNodes()) {
    nodes[i] = n;
    ++i;
  }
  bool stopfor = false;
#ifdef _OPENMP
  #pragma omp parallel for private(i) schedule(dynamic, 1)
#endif

  for (i = 0; i < static_cast<int>(nbNodes); ++i) {
    if (stopfor) continue;

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
    node n = nodes[i];
    MutableContainer<unsigned int> distance;
    maxDistance(graph, n, distance, UNDIRECTED);

    for (size_t j = 0; j < nbNodes; ++j) {
      node itn = nodes[j];
      unsigned int d =  distance.get(itn.id);

      if (itn != n && d != UINT_MAX) {
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
double tlp::averageClusteringCoefficient(const Graph *graph,
    PluginProgress * pluginProgress) {
  double sum=0;
  MutableContainer<double> clusters;
  tlp::clusteringCoefficient(graph, clusters, UINT_MAX, pluginProgress);
  node n;
  forEach(n, graph->getNodes())
  sum += clusters.get(n.id);
  return sum / double(graph->numberOfNodes());
}
//================================================================
unsigned int tlp::maxDegree(const Graph *graph) {
  unsigned int maxdeg = 0;
  node n;
  forEach(n, graph->getNodes())
  maxdeg = std::max(maxdeg, graph->deg(n));

  return maxdeg;
}
//================================================================
unsigned int tlp::minDegree(const Graph *graph) {
  unsigned int mindeg = graph->numberOfNodes();
  node n;
  forEach(n, graph->getNodes())
  mindeg = std::min(mindeg, graph->deg(n));

  return mindeg;
}
//================================================================
void tlp::reachableNodes(const Graph *graph, const node startNode,
                         std::set<node> &result, unsigned int maxDistance,
                         EDGE_TYPE direction) {
  deque<node> fifo;
  MutableContainer<bool> visited;
  MutableContainer<unsigned int> distance;
  visited.setAll(false);
  distance.setAll(graph->numberOfNodes());
  fifo.push_back(startNode);
  visited.set(startNode.id, true);
  distance.set(startNode.id, 0);

  while(!fifo.empty()) {
    node current = fifo.front();
    fifo.pop_front();

    if (distance.get(current.id) < maxDistance) {
      Iterator<node> *itN = getIt(graph, current, direction);

      while(itN->hasNext()) {
        node itn = itN->next();

        if (!visited.get(itn.id)) {
          fifo.push_back(itn);
          result.insert(itn);
          visited.set(itn.id,true);
          distance.set(itn.id, distance.get(current.id) + 1);
        }
      }

      delete itN;
    }
  }
}
//=================================================
void tlp::clusteringCoefficient(const Graph *graph,
                                MutableContainer<double>& clusters,
                                unsigned int maxDepth, PluginProgress *) {
  node n;
  forEach(n, graph->getNodes()) {
    set<node> reachables;
    reachableNodes(graph, n, reachables, maxDepth);
    double nbEdge=0; //e(N_v)*2$
    set<node>::iterator itSN=reachables.begin();

    for (; itSN !=reachables.end(); ++itSN) {
      node itn = *itSN;
      Iterator<edge> *itE = graph->getInOutEdges(itn);

      while (itE->hasNext()) {
        pair<node, node> eEnds = graph->ends(itE->next());

        if ( (reachables.find(eEnds.first)!=reachables.end()) &&
             (reachables.find(eEnds.second)!=reachables.end())) {
          ++nbEdge;
        }
      }

      delete itE;
    }

    double nNode = reachables.size(); //$|N_v|$

    if (reachables.size()>1) {
      //$e(N_v)/(\frac{k*(k-1)}{2}}$
      clusters.set(n.id, double(nbEdge)/(nNode*(nNode-1)));
    }
    else
      clusters.set(n.id, 0.);
  }
}
//==================================================
void tlp::dagLevel (const Graph *graph, MutableContainer<unsigned int>& level,
                    PluginProgress*) {
  MutableContainer<unsigned int> totreat;
  deque<node> fifo;
  //===============================================
  node itn;
  forEach(itn, graph->getNodes()) {
    unsigned int indegree = graph->indeg(itn);

    if (indegree==0) {
      fifo.push_back(itn);
      level.set(itn.id, 0);
    }
    else
      totreat.set(itn.id, indegree - 1);
  }

  //==============================================
  while (!fifo.empty()) {
    node current = fifo.front();
    fifo.pop_front();
    node child;
    unsigned int curLevel = level.get(current.id) + 1;
    forEach(child, graph->getOutNodes(current)) {
      if (totreat.get(child.id) > 0)
        totreat.set(child.id, totreat.get(child.id)-1);
      else {
        level.set(child.id, curLevel);
        fifo.push_back(child);
      }
    }
  }
}
