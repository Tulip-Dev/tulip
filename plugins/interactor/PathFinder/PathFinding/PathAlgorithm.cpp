#include "PathAlgorithm.h"

#include <tulip/BooleanProperty.h>
#include <tulip/DoubleProperty.h>
#include <tulip/MutableContainer.h>
#include <tulip/ForEach.h>
#include <tulip/Graph.h>

#include "Dikjstra/Dikjstra.h"
#include "DFS/DFS.h"

#define SMALLEST_WEIGHT 1.E-6

using namespace tlp;
using namespace std;

double computePathLength(BooleanProperty *result, MutableContainer<double> &weights) {
  double retVal(0);
  Graph *graph(result->getGraph());
  edge e;
  forEach(e, graph->getEdges()) {
    if (result->getEdgeValue(e))
      retVal += weights.get(e.id);
  }
  return retVal;
}

bool PathAlgorithm::computePath(Graph *graph, PathType pathType, EdgeOrientation edgesOrientation, node src, node tgt, BooleanProperty *result, DoubleProperty *weights, double tolerance) {
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
  MutableContainer<double> weightsContainer;

  if (!weights) {
    edge e;
    forEach(e, graph->getEdges())
    weightsContainer.set(e.id, SMALLEST_WEIGHT);
  }
  else {
    edge e;
    forEach(e, graph->getEdges()) {
      double val(weights->getEdgeValue(e));

      if (val == 0)
        weightsContainer.set(e.id, SMALLEST_WEIGHT);
      else
        weightsContainer.set(e.id, val);
    }
  }

  set<node> focus;
  vector<node> vNodes;
  DoubleProperty *depth = new DoubleProperty(graph);
  Dikjstra dikjstra;
  dikjstra.initDikjstra(graph, 0, src, edgesOrientation, weightsContainer, 0, focus);

  bool retVal = false;

  switch (pathType) {
  case AllShortest:
    retVal = dikjstra.searchPaths(tgt, result, depth);
    break;

  case OneShortest:
    retVal = dikjstra.searchPath(tgt, result, vNodes, depth);
    break;

  case AllPaths:
    retVal = dikjstra.searchPath(tgt, result, vNodes, depth);

    if (retVal) {
      double pathLength;

      if (tolerance == DBL_MAX)
        pathLength=DBL_MAX;
      else {
        pathLength = computePathLength(result, weightsContainer);
        pathLength *= tolerance;
      }

      if (tolerance > 1) { // We only compute the other paths if the tolerance is greater than 1. Meaning that the user doesn't want only the shortest path.
        result->setAllNodeValue(false);
        result->setAllEdgeValue(false);
        DoubleProperty *dists = new DoubleProperty(result->getGraph());
        DFS d(graph, result, dists, tgt, weightsContainer, edgesOrientation, pathLength);
        retVal = d.searchPaths(src);
        delete dists;
      }
    }

    break;
  }

  delete depth;
  return retVal;
}
