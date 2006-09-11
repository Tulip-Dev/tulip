//-*-c++-*-
#ifndef _TLPGRAPHMEASEURE_H
#define _TLPGRAPHMEASEURE_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "Node.h"
#include "MutableContainer.h"

namespace tlp {

  class Graph;
  TLP_SCOPE double averagePathLength(Graph *);
  TLP_SCOPE double averageCluster(Graph *);
  TLP_SCOPE unsigned int maxDegree(Graph *);
  TLP_SCOPE unsigned int minDegree(Graph *);
  TLP_SCOPE unsigned int maxDistance(Graph *sg, node n, MutableContainer<unsigned int> &distance, int direction = 2);
}
#endif
