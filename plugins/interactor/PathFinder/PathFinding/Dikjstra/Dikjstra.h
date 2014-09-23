/**
 *
 * This file is part of Tulip (www.tulip-software.org)
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

#ifndef DIKJSTRA_H
#define DIKJSTRA_H
#include <vector>
#include <set>
#include <map>
#include <cmath>
#include <cstdlib>
#include <limits.h>
#include <tulip/Graph.h>
#include <tulip/MutableContainer.h>
#include <tulip/Vector.h>
#include <tulip/ForEach.h>
#include <tulip/LayoutProperty.h>
#include <tulip/DoubleProperty.h>

#include "../PathAlgorithm.h"

namespace tlp {

class Dikjstra: public PathAlgorithm {
public:
  //============================================================
  void initDikjstra(const tlp::Graph * const graph,
                    const tlp::Graph * const forbiddenNodes,
                    tlp::node src,
                    EdgeOrientation directed,
                    const tlp::MutableContainer<double> &weights,
                    double maxDist,
                    const std::set<tlp::node> &focus);
  //========================================================
  bool searchPaths(tlp::node n, tlp::BooleanProperty *result, tlp::DoubleProperty *depth);
  //=========================================================
  bool searchPath(tlp::node n, tlp::BooleanProperty *result, std::vector<tlp::node> &vNodes, tlp::DoubleProperty *preference);
  //=============================================================
private:
  void internalSearchPaths(tlp::node n, tlp::BooleanProperty *result, tlp::DoubleProperty *depth);
  //=========================================================
  struct DikjstraElement {
    DikjstraElement ( const double dist = DBL_MAX, const tlp::node previous = tlp::node(), const tlp::node n = tlp::node()) :
      dist(dist),
      previous(previous),
      n(n) {
    }
    bool operator==(const DikjstraElement &b) const {
      return n == b.n;
    }
    bool operator!=(const DikjstraElement &b) const {
      return n != b.n;
    }
    double dist;
    tlp::node   previous;
    tlp::node   n;
    std::vector<tlp::edge> usedEdge;
  };

  struct LessDikjstraElement {
    bool operator()(const DikjstraElement * const a, const DikjstraElement * const b ) {
      if (fabs(a->dist - b->dist) > 1.E-9) {
        return (a->dist < b->dist);
      }
      else {
        return (a->n.id < b->n.id);
      }
    }
  };

  tlp::Graph const *graph;
  tlp::Graph const *forbiddenNodes;
  tlp::node  src;
  tlp::MutableContainer<bool> usedEdges;

public:
  tlp::MutableContainer<double> nodeDistance;
};
}
#endif // DIKJSTRA_H
