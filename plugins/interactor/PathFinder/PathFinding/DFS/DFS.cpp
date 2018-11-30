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

#include "DFS.h"

#include <stack>
#include <tulip/BooleanProperty.h>
#include <tulip/DoubleProperty.h>

using namespace tlp;
using namespace std;

DFS::DFS(Graph *graph, BooleanProperty *result, node tgt,
         const EdgeStaticProperty<double> &eWeights, EdgeOrientation edgesOrientation,
         double maxDist)
    : graph(graph), result(result), tgt(tgt), weights(eWeights), currentDist(0),
      edgesOrientation(edgesOrientation), maxDist(maxDist) {
#ifndef NDEBUG
  assert(graph->getRoot() == result->getGraph()->getRoot());
#endif /* NDEBUG */
}

bool DFS::searchPaths(node src) {
  DoubleProperty dists(result->getGraph());
  dists.setAllNodeValue(DBL_MAX);

  BooleanProperty visitable(graph);
  visitable.setAllNodeValue(true);

  return computeSearchPaths(src, &visitable, &dists);
}

bool DFS::computeSearchPaths(node src, BooleanProperty *visitable, DoubleProperty *dists) {
  if (!visitable->getNodeValue(src))
    return false;

  if (dists->getNodeValue(src) != DBL_MAX && currentDist + dists->getNodeValue(src) > maxDist)
    return false;

  if (currentDist > maxDist)
    return false;

  else if (src == tgt || result->getNodeValue(src)) {
    double distLeft(0);

    if (result->getNodeValue(src))
      distLeft = dists->getNodeValue(src);

    node nd(src);

    for (vector<edge>::reverse_iterator it = path.rbegin(); it != path.rend(); ++it) {
      edge e(*it);
      node opposite(graph->opposite(e, nd));
      result->setEdgeValue(e, true);
      result->setNodeValue(opposite, true);
      result->setNodeValue(nd, true);
      dists->setNodeValue(nd, min<double>(distLeft, dists->getNodeValue(nd)));
      distLeft += weights.getEdgeValue(e);
      nd = opposite;
    }

    dists->setNodeValue(nd, min<double>(distLeft, dists->getNodeValue(nd)));

    return true;
  }

  bool res = false;
  visitable->setNodeValue(src, false);

  Iterator<edge> *edgeIt = nullptr;

  switch (edgesOrientation) {
  case Undirected:
    edgeIt = graph->getInOutEdges(src);
    break;

  case Directed:
    edgeIt = graph->getOutEdges(src);
    break;

  case Reversed:
    edgeIt = graph->getInEdges(src);
    break;
  }

  for (auto e : edgeIt) {
    currentDist += weights.getEdgeValue(e);
    path.push_back(e);
    res |= computeSearchPaths(graph->opposite(e, src), visitable, dists);
    path.pop_back();
    currentDist -= weights.getEdgeValue(e);
  }

  visitable->setNodeValue(src, true);
  return res;
}
