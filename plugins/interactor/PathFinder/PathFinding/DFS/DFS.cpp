/**
 *
 * This file is part of Tulip (https://tulip.labri.fr)
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
#include <tulip/PluginProgress.h>
#include <tulip/Perspective.h>

using namespace tlp;
using namespace std;

#define PROGRESS_TIME_STEP 1000
DFS::DFS(Graph *graph, BooleanProperty *result, node tgt,
         const EdgeStaticProperty<double> &eWeights, EdgeOrientation edgesOrientation,
         double maxDist)
    : graph(graph), result(result), tgt(tgt), weights(eWeights), currentDist(0),
      edgesOrientation(edgesOrientation), maxDist(maxDist), progress(nullptr), nbPaths(0) {
#ifndef NDEBUG
  assert(graph->getRoot() == result->getGraph()->getRoot());
#endif /* NDEBUG */
}

bool DFS::searchPaths(node n) {
  DoubleProperty dists(result->getGraph());
  dists.setAllNodeValue(DBL_MAX);

  BooleanProperty visitable(graph);
  visitable.setAllNodeValue(true);

  // keep paths origin
  src = n;
  // initialize progress timer
  // progress initialization is delayed until the first progress step
  // has elapsed
  lastProgressTime = QTime::currentTime();
  auto result = computeSearchPaths(n, &visitable, &dists);
  delete progress;
  return result;
}

bool DFS::computeSearchPaths(node n, BooleanProperty *visitable, DoubleProperty *dists) {
  if (!visitable->getNodeValue(n))
    return false;

  if ((maxDist != DBL_MAX) &&
      (((dists->getNodeValue(n) != DBL_MAX) && (currentDist + dists->getNodeValue(n) > maxDist)) ||
       (currentDist > maxDist)))
    return false;

  if (n == tgt || result->getNodeValue(n)) {
    double distLeft(0);

    if ((maxDist != DBL_MAX) && result->getNodeValue(n))
      distLeft = dists->getNodeValue(n);

    for (vector<edge>::reverse_iterator it = path.rbegin(); it != path.rend(); ++it) {
      edge e(*it);
      node opposite(graph->opposite(e, n));
      result->setEdgeValue(e, true);
      result->setNodeValue(opposite, true);
      result->setNodeValue(n, true);
      if (maxDist != DBL_MAX) {
        dists->setNodeValue(n, min<double>(distLeft, dists->getNodeValue(n)));
        distLeft += weights.getEdgeValue(e);
      }
      n = opposite;
    }

    if (maxDist != DBL_MAX)
      dists->setNodeValue(n, min<double>(distLeft, dists->getNodeValue(n)));

    // update the number of paths found
    ++nbPaths;
    updateProgressComment();
    return true;
  }

  bool res = false;
  visitable->setNodeValue(n, false);

  Iterator<edge> *edgeIt = nullptr;

  switch (edgesOrientation) {
  case Undirected:
    edgeIt = graph->getInOutEdges(n);
    break;

  case Directed:
    edgeIt = graph->getOutEdges(n);
    break;

  case Reversed:
    edgeIt = graph->getInEdges(n);
    break;
  }

  for (auto e : edgeIt) {
    // check for progress interruption
    if (lastProgressTime.msecsTo(QTime::currentTime()) >= PROGRESS_TIME_STEP) {
      if (progress == nullptr) {
        // initialize plugin progress
        progress = Perspective::instance()->progress();
        std::ostringstream oss;
        oss << "Searching all ";
        switch (edgesOrientation) {
        case Undirected:
          oss << "undirected ";
          break;

        case Directed:
          oss << "directed ";
          break;

        case Reversed:
          oss << "reverse ";
          break;
        }
        oss << "paths from node #" << src.id << " to node #" << tgt.id;
        progress->setTitle(oss.str());
        updateProgressComment();
        progress->showPreview(false);
        progress->showText(false);
      }
      // (0, 0) means "show a busy progress bar"
      if (progress->progress(0, 0) != TLP_CONTINUE)
        break;
      lastProgressTime = QTime::currentTime();
    }
    if (maxDist != DBL_MAX)
      currentDist += weights.getEdgeValue(e);
    path.push_back(e);
    res |= computeSearchPaths(graph->opposite(e, n), visitable, dists);
    path.pop_back();
    if (progress && (progress->state() != TLP_CONTINUE))
      break;
    if (maxDist != DBL_MAX)
      currentDist -= weights.getEdgeValue(e);
  }

  if (progress) {
    switch (progress->state()) {
    case TLP_CANCEL:
      return false;
    case TLP_STOP:
      return result->getNodeValue(tgt);
    default:
      break;
    }
  }
  visitable->setNodeValue(n, true);
  return res;
}

void DFS::updateProgressComment() {
  if (progress) {
    switch (nbPaths) {
    case 0:
      progress->setComment("No path found...");
      break;
    case 1:
      progress->setComment("One path found...");
      break;
    default:
      std::ostringstream oss;
      oss << nbPaths << " paths found...";
      progress->setComment(oss.str());
    }
  }
}
