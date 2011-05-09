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

#ifndef TULIP_TRICONNECTEDTEST_H
#define TULIP_TRICONNECTEDTEST_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <tulip/tuliphash.h>
#include <tulip/Observable.h>
#include <tulip/ObservableGraph.h>

namespace tlp {

class Graph;

/** \addtogroup graph_test */ 
/*@{*/
/// class for testing if a graph is triconnected
 class TLP_SCOPE TriconnectedTest : private GraphObserver, private Observable {
public:


  /**
   * Returns true if the graph is triconnected (i.e. a connected graph such that deleting any two nodes (and incident edges)
   * results in a graph that is still connected), false otherwise.
   */
  static bool isTriconnected(Graph *graph);

private:
  bool compute(Graph *);
  // override some GraphObserver methods
  void addEdge(Graph *,const edge);
  void delEdge(Graph *,const edge);
  void reverseEdge(Graph *,const edge);
  void addNode(Graph *,const node);
  void delNode(Graph *,const node);
  void destroy(Graph *);
  // override Observable::treatEvent
  void treatEvent(const Event&);
  TriconnectedTest();
  static TriconnectedTest * instance;
  TLP_HASH_MAP<unsigned long,bool> resultsBuffer;
};
/*@}*/

}
#endif
