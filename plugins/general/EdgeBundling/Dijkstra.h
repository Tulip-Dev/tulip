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

#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include <vector>
#include <set>
#include <climits>
#include <tulip/Graph.h>
#include <tulip/MutableContainer.h>
#include <tulip/Vector.h>
#include <tulip/LayoutProperty.h>
#include <tulip/StaticProperty.h>
#include <tulip/vectorgraph.h>
#include <tulip/ParallelTools.h>

// we need a lock to protect the allocation/free
// of needed VectorGraph properties
TLP_DECLARE_GLOBAL_LOCK(DijkstraProps);

class Dijkstra {

public:
  //============================================================
  Dijkstra() {
    TLP_GLOBALLY_LOCK_SECTION(DijkstraProps) {
      graph.alloc(forbiddenNodes);
      graph.alloc(usedEdges);
      graph.alloc(nodeDistance);
      graph.alloc(resultNodes);
      graph.alloc(resultEdges);
      graph.alloc(mapDik);
    }
    TLP_GLOBALLY_UNLOCK_SECTION(DijkstraProps);
  }

  ~Dijkstra() {
    TLP_GLOBALLY_LOCK_SECTION(DijkstraProps) {
      graph.free(forbiddenNodes);
      graph.free(usedEdges);
      graph.free(nodeDistance);
      graph.free(resultNodes);
      graph.free(resultEdges);
      graph.free(mapDik);
    }
    TLP_GLOBALLY_UNLOCK_SECTION(DijkstraProps);
  }

  static void loadGraph(const tlp::Graph *src) {

    graph.delAllNodes();

    graph.reserveNodes(src->numberOfNodes());
    graph.reserveEdges(src->numberOfEdges());

    ndik2tlp.setAll(tlp::node());
    edik2tlp.setAll(tlp::edge());
    ntlp2dik.setAll(tlp::node());
    etlp2dik.setAll(tlp::edge());

    for (auto n : src->nodes()) {
      tlp::node newNode = graph.addNode();
      ntlp2dik.set(n, newNode);
      ndik2tlp[newNode] = n;
      graph.reserveAdj(newNode, src->deg(n));
    }

    for (auto e : src->edges()) {
      auto eEnds = src->ends(e);
      tlp::edge tmp = graph.addEdge(ntlp2dik.get(eEnds.first), ntlp2dik.get(eEnds.second));
      etlp2dik.set(e, tmp);
      edik2tlp[tmp] = e;
    }
  }

  void initDijkstra(const tlp::Graph *const forbiddenNodes, tlp::node src,
                    const tlp::EdgeStaticProperty<double> &weights,
                    const std::set<tlp::node> &focus);

  //========================================================
  void searchPaths(tlp::node n, tlp::EdgeStaticProperty<unsigned int> &depth);
  void searchPath(tlp::node n, std::vector<tlp::node> &vNodes);
  //=============================================================
private:
  struct DijkstraElement {
    DijkstraElement(const double dist = DBL_MAX, const tlp::node previous = tlp::node(),
                    const tlp::node n = tlp::node())
        : dist(dist), previous(previous), n(n) {}
    //=========================================================
    bool operator==(const DijkstraElement &b) const {
      return n == b.n;
    }
    bool operator!=(const DijkstraElement &b) const {
      return n != b.n;
    }
    double dist;
    tlp::node previous;
    tlp::node n;
    std::vector<tlp::edge> usedEdge;
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

  tlp::node src;

  tlp::NodeProperty<double> nodeDistance;
  tlp::NodeProperty<bool> forbiddenNodes;
  tlp::EdgeProperty<bool> usedEdges;
  tlp::NodeProperty<bool> resultNodes;
  tlp::EdgeProperty<bool> resultEdges;
  tlp::NodeProperty<DijkstraElement *> mapDik;

  static tlp::VectorGraph graph;
  static tlp::NodeProperty<tlp::node> ndik2tlp;
  static tlp::EdgeProperty<tlp::edge> edik2tlp;
  static tlp::MutableContainer<tlp::node> ntlp2dik;
  static tlp::MutableContainer<tlp::edge> etlp2dik;
  static bool _initB;

  static bool initG() {
    graph.alloc(ndik2tlp);
    graph.alloc(edik2tlp);
    return true;
  }
};

#endif // DIJKSTRA_H
