//-*-c++-*-
#ifndef _TLPGRAPHMEASEURE_H
#define _TLPGRAPHMEASEURE_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "Node.h"
#include "MutableContainer.h"

class SuperGraph;

namespace tlp {
  TLP_SCOPE double averagePathLength(SuperGraph *);
  TLP_SCOPE double averageCluster(SuperGraph *);
  TLP_SCOPE unsigned int maxDegree(SuperGraph *);
  TLP_SCOPE unsigned int minDegree(SuperGraph *);
  TLP_SCOPE unsigned int maxDistance(SuperGraph *graph, node n, MutableContainer<unsigned int> &distance, int direction = 2, unsigned int max_dist = UINT_MAX);
};

#endif
