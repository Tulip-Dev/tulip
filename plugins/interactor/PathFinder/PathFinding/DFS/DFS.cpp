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
#include <tulip/PluginProgress.h>
#include <tulip/Perspective.h>

using namespace tlp;
using namespace std;

#define PROGRESS_MAX_STEPS 10
#define PROGRESS_TIME_STEP 1500
DFS::DFS(Graph *graph, BooleanProperty *result, node tgt,
         const EdgeStaticProperty<double> &eWeights, EdgeOrientation edgesOrientation,
         double maxDist)
    : graph(graph), result(result), tgt(tgt), weights(eWeights), currentDist(0),
      edgesOrientation(edgesOrientation), maxDist(maxDist), progress(nullptr), progressStep(0),
      nbPaths(0), progressStepIncrement(1) {
#ifndef NDEBUG
  assert(graph->getRoot() == result->getGraph()->getRoot());
#endif /* NDEBUG */
}

bool DFS::searchPaths(node src) {
  DoubleProperty dists(result->getGraph());
  dists.setAllNodeValue(DBL_MAX);

  BooleanProperty visitable(graph);
  visitable.setAllNodeValue(true);

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
  progress->setComment("No path found...");
  progress->showPreview(false);
  progress->showText(false);
  lastProgressTime = QTime::currentTime();
  auto result = computeSearchPaths(src, &visitable, &dists);
  delete progress;
  return result;
}

bool DFS::computeSearchPaths(node src, BooleanProperty *visitable, DoubleProperty *dists) {
  if (!visitable->getNodeValue(src))
    return false;

  if ((maxDist != DBL_MAX) && (((dists->getNodeValue(src) != DBL_MAX) &&
                                (currentDist + dists->getNodeValue(src) > maxDist)) ||
                               (currentDist > maxDist)))
    return false;

  if (src == tgt || result->getNodeValue(src)) {
    double distLeft(0);

    if ((maxDist != DBL_MAX) && result->getNodeValue(src))
      distLeft = dists->getNodeValue(src);

    node nd(src);

    for (vector<edge>::reverse_iterator it = path.rbegin(); it != path.rend(); ++it) {
      edge e(*it);
      node opposite(graph->opposite(e, nd));
      result->setEdgeValue(e, true);
      result->setNodeValue(opposite, true);
      result->setNodeValue(nd, true);
      if (maxDist != DBL_MAX) {
        dists->setNodeValue(nd, min<double>(distLeft, dists->getNodeValue(nd)));
        distLeft += weights.getEdgeValue(e);
      }
      nd = opposite;
    }

    if (maxDist != DBL_MAX)
      dists->setNodeValue(nd, min<double>(distLeft, dists->getNodeValue(nd)));

    // update the number of paths found
    if (++nbPaths == 1)
      progress->setComment("One path found...");
    else {
      std::ostringstream oss;
      oss << nbPaths << " paths found...";
      progress->setComment(oss.str());
    }
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
    // check for progress interruption
    if (lastProgressTime.msecsTo(QTime::currentTime()) >= PROGRESS_TIME_STEP) {
      if (progress->progress(progressStep, PROGRESS_MAX_STEPS) != TLP_CONTINUE)
        break;
      if (progressStep == PROGRESS_MAX_STEPS - 1)
        progressStepIncrement = -1;
      else if (progressStep == 1)
        progressStepIncrement = 1;
      progressStep += progressStepIncrement;
      lastProgressTime = QTime::currentTime();
    }
    if (maxDist != DBL_MAX)
      currentDist += weights.getEdgeValue(e);
    path.push_back(e);
    res |= computeSearchPaths(graph->opposite(e, src), visitable, dists);
    path.pop_back();
    if (progress->state() != TLP_CONTINUE)
      break;
    if (maxDist != DBL_MAX)
      currentDist -= weights.getEdgeValue(e);
  }

  switch (progress->state()) {
  case TLP_CANCEL:
    return false;
  case TLP_STOP:
    return result->getNodeValue(tgt);
  default:
    visitable->setNodeValue(src, true);
  }
  return res;
}
