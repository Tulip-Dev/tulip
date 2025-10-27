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

///@cond DOXYGEN_HIDDEN
#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include <vector>
#include <stack>
#include <list>
#include <tulip/tuliphash.h>
#include <functional>
#include <tulip/Graph.h>
#include <tulip/BooleanProperty.h>
#include <tulip/StaticProperty.h>
#include <tulip/MutableContainer.h>
#include <tulip/GraphTools.h>

namespace tlp {

class Dijkstra {
public:
  //============================================================
  Dijkstra(const Graph *const graph, node src, const EdgeStaticProperty<double> &weights,
           NodeStaticProperty<double> &nodeDistance, EDGE_TYPE direction,
           std::stack<node> *qN = nullptr, MutableContainer<int> *nP = nullptr);
  //========================================================
  bool searchPaths(node n, BooleanProperty *result);
  //=========================================================
  bool searchPath(node n, BooleanProperty *result);
  //=============================================================
  bool ancestors(tlp_hash_map<node, std::list<node>> &result);

private:
  void internalSearchPaths(node n, BooleanProperty *result);
  //=========================================================
  struct DijkstraElement {
    DijkstraElement(const double dist = DBL_MAX, const node previous = node(),
                    const node n = node())
        : dist(dist), previous(previous), n(n) {}
    bool operator==(const DijkstraElement &b) const {
      return n == b.n;
    }
    bool operator!=(const DijkstraElement &b) const {
      return n != b.n;
    }
    double dist;
    node previous;
    node n;
    std::vector<edge> usedEdge;
  };

  struct LessDijkstraElement {
    bool operator()(const DijkstraElement *const a, const DijkstraElement *const b) const {
      if (fabs(a->dist - b->dist) > 1.E-9) {
        return (a->dist < b->dist);
      } else {
        return (a->n.id < b->n.id);
      }
    }
  };

  Graph const *graph;
  node src;
  MutableContainer<bool> usedEdges;
  NodeStaticProperty<double> &nodeDistance;
  std::stack<node> *queueNodes;
  MutableContainer<int> *numberOfPaths;
};
} // namespace tlp

#endif // DIJKSTRA_H
///@endcond
