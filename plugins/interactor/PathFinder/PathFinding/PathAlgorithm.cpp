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

#include <queue>

#include "PathAlgorithm.h"

#include <tulip/BooleanProperty.h>
#include <tulip/DoubleProperty.h>
#include <tulip/MutableContainer.h>
#include <tulip/Graph.h>
#include <tulip/GraphParallelTools.h>
#include <tulip/GraphTools.h>

#include "DFS/DFS.h"

#define SMALLEST_WEIGHT 1.E-6

using namespace tlp;
using namespace std;

static double computePathLength(BooleanProperty *result, EdgeStaticProperty<double> &weights) {
  double retVal(0);
  Graph *graph(result->getGraph());
  auto ite = result->getNonDefaultValuatedEdges(graph);
  while (ite->hasNext()) {
    retVal += weights.getEdgeValue(ite->next());
  }
  delete ite;
  return retVal;
}

// The basic idea is to try to first eliminate all nodes
// which cannot belong to path between src and tgt, that is those
// which belong to "dead end" paths.
static Graph *computeAllPathsGraph(Graph *graph, node src, node tgt,
                                   PathAlgorithm::EdgeOrientation edgesOrientation) {
  Graph *sg;

  // first step to extract the connected component owning src and tgt
  {
    NodeStaticProperty<bool> visited(graph, false);
    std::queue<node> nodesQueue;
    visited[src] = true;
    nodesQueue.push(src);
    unsigned int nbVisited = 1;

    // BFS loop from src
    // to extract the connected component owning src
    while (!nodesQueue.empty()) {
      node n = nodesQueue.front();
      nodesQueue.pop();
      // loop on all neighbours
      for (auto nn : graph->getInOutNodes(n)) {
        unsigned int nnPos = graph->nodePos(nn);
        // check if neighbour has been visited
        if (!visited[nnPos]) {
          // mark neighbour as already visited
          visited[nnPos] = true;
          // add in queue for further deeper exploration
          nodesQueue.push(nn);
          ++nbVisited;
        }
      }
    }

    // check if tgt is in the same connected component
    if (!visited[tgt])
      return nullptr;

    // now delete non visited nodes from a clone
    sg = graph->addCloneSubGraph();

    if (nbVisited < graph->numberOfNodes()) {
      TLP_MAP_NODES_AND_INDICES(graph, [&](node n, unsigned int i) {
        if (!visited[i])
          sg->delNode(n);
      });
    }
  }

  // second step, remove all leaf nodes different from src and tgt
  {
    std::vector<node> v = sg->nodes();
    for (node n : v) {
      while ((sg->deg(n) == 1) && (n != src) && (n != tgt)) {
        // get current node's neighbour
        auto it = sg->getInOutNodes(n);
        node nn = it->next();
        delete it;
        // delete current node
        sg->delNode(n);
        // go further in the "dead end" path with neighbour
        n = nn;
      }
    }
  }

  // special step when Directed or Reversed
  if (edgesOrientation != PathAlgorithm::Undirected) {
    if (edgesOrientation == PathAlgorithm::Reversed) {
      // switch src and tgt
      // to consider only the Directed case
      node n(src);
      src = tgt;
      tgt = n;
      edgesOrientation = PathAlgorithm::Directed;
    }
    // check if there is a possible path
    if (sg->outdeg(src) == 0 || sg->indeg(tgt) == 0)
      return (graph->delSubGraph(sg), nullptr);

    // we recursively delete all nodes belonging to a "dead end" path
    std::vector<node> v = sg->nodes();
    for (node n : v) {
      if (!sg->isElement(n))
        continue;
      // a node belongs to a "dead end" path
      // if it is a "sink" node and is different from tgt,
      if (sg->outdeg(n) == 0 && n != tgt) {
        if (n == src)
          // no possible path
          return (graph->delSubGraph(sg), nullptr);
        // n is the end of a "dead end" path
        std::queue<node> deadEndPath;
        deadEndPath.push(n);
        while (!deadEndPath.empty()) {
          n = deadEndPath.front();
          deadEndPath.pop();
          // enqueue current node's in-neighbours
          // of which it is the only out-neighbour
          Iterator<node> *itn = sg->getInNodes(n);
          while (itn->hasNext()) {
            node nn = itn->next();
            if (nn != tgt && sg->outdeg(nn) == 1) {
              if (nn == src) {
                delete itn;
                // no possible path
                return (graph->delSubGraph(sg), nullptr);
              }
              deadEndPath.push(nn);
            }
          }
          delete itn;
          // n can now be safely deleted
          sg->delNode(n);
        }
        continue;
      }
      // a node belongs also to a "dead end" path
      // if it is a "source" node and is different from src,
      if (sg->indeg(n) == 0 && n != src) {
        if (n == tgt)
          // no possible path
          return (graph->delSubGraph(sg), nullptr);
        // n is the beginning of a dead end path
        std::queue<node> deadEndPath;
        deadEndPath.push(n);
        while (!deadEndPath.empty()) {
          n = deadEndPath.front();
          deadEndPath.pop();
          // enqueue current node's out-neighbours
          // of which it is the only in-neighbour
          Iterator<node> *itn = sg->getOutNodes(n);
          while (itn->hasNext()) {
            node nn = itn->next();
            if (nn != src && sg->indeg(nn) == 1) {
              if (nn == tgt) {
                delete itn;
                // no possible path
                return (graph->delSubGraph(sg), nullptr);
              }
              deadEndPath.push(nn);
            }
          }
          delete itn;
          // n can now be safely deleted
          sg->delNode(n);
        }
      }
    }
  }
  return sg;
}

bool PathAlgorithm::computePath(Graph *graph, PathType pathType, EdgeOrientation edgesOrientation,
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

  bool retVal = false;
  tlp::ShortestPathType spt;

  if (pathType == AllShortest) {
    switch (edgesOrientation) {
    case Directed:
      spt = ShortestPathType::AllDirectedPaths;
      break;
    case Undirected:
      spt = ShortestPathType::AllPaths;
      break;
    case Reversed:
    default:
      spt = ShortestPathType::AllReversedPaths;
    }
  } else {
    switch (edgesOrientation) {
    case Directed:
      spt = ShortestPathType::OneDirectedPath;
      break;
    case Undirected:
      spt = ShortestPathType::OnePath;
      break;
    case Reversed:
    default:
      spt = ShortestPathType::OneReversedPath;
    }
  }
  // allow undo
  graph->push();
  retVal = ((pathType == AllPaths) && (tolerance == DBL_MAX)) ||
           selectShortestPaths(graph, src, tgt, spt, weights, result);
  // We only compute the other paths if the tolerance is greater than 1.
  // Meaning that the user doesn't want only the shortest path.
  if (pathType == AllPaths && retVal && tolerance > 1) {
    // eliminates nodes belonging to "dead end" paths
    Graph *sg = computeAllPathsGraph(graph, src, tgt, edgesOrientation);
    if (sg == nullptr)
      retVal = false;
    else {
      EdgeStaticProperty<double> eWeights(sg, SMALLEST_WEIGHT);
      if (tolerance != DBL_MAX) {
        if (weights) {
          auto fn = [&](edge e, unsigned int i) {
            double val(weights->getEdgeValue(e));
            if (val)
              eWeights[i] = val;
          };
          TLP_PARALLEL_MAP_EDGES_AND_INDICES(graph, fn);
        }
        tolerance *= computePathLength(result, eWeights);
        result->setAllNodeValue(false);
        result->setAllEdgeValue(false);
      }
      // finally do a DFS
      DFS d(sg, result, tgt, eWeights, edgesOrientation, tolerance);
      retVal = d.searchPaths(src);
      graph->delSubGraph(sg);
    }
  }
  if (!retVal)
    // nothing to undo
    graph->pop();
  return retVal;
}
