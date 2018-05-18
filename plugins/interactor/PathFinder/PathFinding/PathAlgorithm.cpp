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

#include "PathAlgorithm.h"

#include <tulip/BooleanProperty.h>
#include <tulip/DoubleProperty.h>
#include <tulip/MutableContainer.h>
#include <tulip/Graph.h>
#include <tulip/GraphParallelTools.h>

#include "Dikjstra/Dikjstra.h"
#include "DFS/DFS.h"

#define SMALLEST_WEIGHT 1.E-6

using namespace tlp;
using namespace std;

double computePathLength(BooleanProperty *result, EdgeStaticProperty<double> &weights) {
  double retVal(0);
  Graph *graph(result->getGraph());
  auto ite = result->getNonDefaultValuatedEdges(graph);
  while(ite->hasNext()) {
    retVal += weights.getEdgeValue(ite->next());
  }
  delete ite;
  return retVal;
}

bool PathAlgorithm::computePath(Graph *graph, PathType pathType,
				EdgeOrientation edgesOrientation,
                                node src, node tgt, BooleanProperty *result,
                                DoubleProperty *weights, double tolerance) {
#ifndef NDEBUG
  assert(graph);
  assert(result);

  if (weights)
    assert(result->getGraph() == weights->getGraph());

  assert(graph->isElement(src));
  assert(graph->isElement(tgt));
  assert(src != tgt);
#endif /* NDEBUG */

  // We always compute Dikjstra as it is used in the all path computation too
  EdgeStaticProperty<double> eWeights(graph);

  if (!weights) {
    eWeights.setAll(SMALLEST_WEIGHT);
  } else {
    auto fn = [&](edge e, unsigned int i) {
      double val(weights->getEdgeValue(e));

      eWeights[i] = val ? val : SMALLEST_WEIGHT;
    };
    TLP_PARALLEL_MAP_EDGES_AND_INDICES(graph, fn);
  }

  set<node> focus;
  vector<node> vNodes;
  Dikjstra dikjstra;
  dikjstra.initDikjstra(graph, nullptr, src, edgesOrientation, eWeights, 0, focus);

  bool retVal = false;

  switch (pathType) {
  case AllShortest:
    retVal = dikjstra.searchPaths(tgt, result);
    break;

  case OneShortest:
    retVal = dikjstra.searchPath(tgt, result, vNodes);
    break;

  case AllPaths:
    retVal = dikjstra.searchPath(tgt, result, vNodes);

    if (retVal) {
      double pathLength;

      if (tolerance == DBL_MAX)
        pathLength = DBL_MAX;
      else {
        pathLength = computePathLength(result, eWeights);
        pathLength *= tolerance;
      }

      if (tolerance > 1) { // We only compute the other paths if the tolerance is greater than 1.
                           // Meaning that the user doesn't want only the shortest path.
        result->setAllNodeValue(false);
        result->setAllEdgeValue(false);
        DoubleProperty dists(result->getGraph());
        DFS d(graph, result, &dists, tgt, eWeights, edgesOrientation, pathLength);
        retVal = d.searchPaths(src);
       }
    }

    break;
  }
  return retVal;
}
