//-*-c++-*-
#ifndef _TLPGRAPHMEASEURE_H
#define _TLPGRAPHMEASEURE_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "Node.h"
#include "MutableContainer.h"
#include "PluginProgress.h"
#include "DoubleProperty.h"

namespace tlp {

  class Graph;
  TLP_SCOPE bool averagePathLength(Graph *, double&, PluginProgress * = 0);
  TLP_SCOPE bool averageCluster(Graph *, double &, PluginProgress * = 0);
  TLP_SCOPE bool computeClusterMetric(Graph *graph, DoubleProperty *prop,
				    unsigned int depth, PluginProgress * = 0);
  TLP_SCOPE bool computeDagLevelMetric(Graph *graph, DoubleProperty *prop, PluginProgress * = 0);
  TLP_SCOPE unsigned int maxDegree(Graph *);
  TLP_SCOPE unsigned int minDegree(Graph *);
  /*
   * Compute the distance from n to all the other nodes in sg and store it into distance, if direction is set
   * to 2 use undirected graph, 0 use directed graph and 1 use reverse directed graph (ie. all edges are reversed)
   * all the edge's weight is set to 1. (it uses a bfs thus the complexity is o(m), m = |E|).
   */
  TLP_SCOPE unsigned int maxDistance(Graph *sg, node n, MutableContainer<unsigned int> &distance, int direction = 2);
}
#endif
