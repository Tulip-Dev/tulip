/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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
#ifndef OUTERPLANARTEST_H
#define OUTERPLANARTEST_H

#include <tulip/tuliphash.h>
#include <tulip/ObservableGraph.h>

namespace tlp {

class Graph;
struct edge;
struct node;

/** \addtogroup graph_test */ 
/*@{*/
/// class for testing the outerplanarity of a graph
class TLP_SCOPE OuterPlanarTest  : private GraphObserver {
public:

  /**
   * Returns true if the graph is outerplanar (i.e. a graph with an embedding
   * in the plane such that all vertices belong to the unbounded face of the embedding),
   * false otherwise.
   */
  static bool isOuterPlanar(Graph *graph);
  
private:
  // override some GraphObserver methods
  void addEdge(Graph *,const edge);
  void delEdge(Graph *,const edge);
  void reverseEdge(Graph *,const edge);
  void addNode(Graph *,const node);
  void delNode(Graph *,const node);
  void destroy(Graph *);
  bool compute(Graph *graph);
  OuterPlanarTest() {}
  static OuterPlanarTest* instance;
  TLP_HASH_MAP<unsigned long, bool> resultsBuffer;
};
/*@}*/
}

#endif 
