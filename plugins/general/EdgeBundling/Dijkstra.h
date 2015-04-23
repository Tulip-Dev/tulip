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

#include <tulip/vectorgraph.h>

using namespace std;

class Dijkstra {

public:
  //============================================================
  Dijkstra() {
#ifdef _OPENMP
    #pragma omp critical(DIKCREATE)
#endif
    {
      graph.alloc(forbiddenNodes);
      graph.alloc(usedEdges);
      graph.alloc(nodeDistance);
      graph.alloc(resultNodes);
      graph.alloc(resultEdges);
      graph.alloc(mapDik);
    }
  }

  ~Dijkstra() {
#ifdef _OPENMP
    #pragma omp critical(DIKCREATE)
#endif
    {
      graph.free(forbiddenNodes);
      graph.free(usedEdges);
      graph.free(nodeDistance);
      graph.free(resultNodes);
      graph.free(resultEdges);
      graph.free(mapDik);
    }
  }



  static void loadGraph(const tlp::Graph *src) {

    graph.delAllNodes();

    graph.reserveNodes(src->numberOfNodes());
    graph.reserveEdges(src->numberOfEdges());

    ndik2tlp.setAll(tlp::node());
    edik2tlp.setAll(tlp::edge());
    ntlp2dik.setAll(tlp::node());
    etlp2dik.setAll(tlp::edge());

    tlp::node n;
    forEach(n, src->getNodes()) {
      tlp::node newNode = graph.addNode();
      ntlp2dik.set(n, newNode);
      ndik2tlp[newNode] = n;
      graph.reserveAdj(newNode, src->deg(n));
    }


    tlp::edge e;
    forEach(e, src->getEdges()) {
      const std::pair<tlp::node, tlp::node>& eEnds = src->ends(e);
      tlp::edge tmp = graph.addEdge(ntlp2dik.get(eEnds.first), ntlp2dik.get(eEnds.second));
      etlp2dik.set(e, tmp);
      edik2tlp[tmp] = e;
    }
  }


  void initDijkstra(const tlp::Graph * const forbiddenNodes,
                    tlp::node src,
                    const tlp::MutableContainer<double> &weights,
                    const std::set<tlp::node> &focus);

  //========================================================
  void searchPaths(tlp::node n, tlp::DoubleProperty *depth);
  void searchPath (tlp::node n, std::vector<tlp::node> &vNodes);
  //=============================================================
private:
  struct DijkstraElement {
    DijkstraElement ( const double dist = DBL_MAX, const tlp::node previous = tlp::node(), const tlp::node n = tlp::node()) :
      dist(dist),
      previous(previous),
      n(n) {
    }
    //=========================================================
    bool operator==(const DijkstraElement &b) const {
      return n == b.n;
    }
    bool operator!=(const DijkstraElement &b) const {
      return n != b.n;
    }
    double dist;
    tlp::node   previous;
    tlp::node   n;
    std::vector<tlp::edge> usedEdge;
  };

  struct LessDijkstraElement {
    bool operator()(const DijkstraElement * const a, const DijkstraElement * const b ) {
      if (fabs(a->dist - b->dist) > 1.E-9) {
        return (a->dist < b->dist);
      }
      else {
        return (a->n.id < b->n.id);
      }
    }
  };



  tlp::node  src;


  tlp::NodeProperty<double> nodeDistance;
  tlp::NodeProperty<bool> forbiddenNodes;
  tlp::EdgeProperty<bool> usedEdges;
  tlp::NodeProperty<bool> resultNodes;
  tlp::EdgeProperty<bool> resultEdges;
  tlp::NodeProperty<DijkstraElement*> mapDik;

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




#endif // DIKJSTRA_H
