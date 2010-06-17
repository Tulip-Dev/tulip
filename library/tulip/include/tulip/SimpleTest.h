/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
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

#ifndef TULIP_SIMPLETEST_H
#define TULIP_SIMPLETEST_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "tulip/tuliphash.h"
#include "tulip/ObservableGraph.h"

namespace tlp {

class Graph;

/** \addtogroup graph_test */ 
/*@{*/
/**
 * class for testing if the graph is simple (ie no self loops and  no multiple or parallel edges)
 */
class TLP_SCOPE SimpleTest : public GraphObserver {
 public: 
  static bool isSimple(Graph *graph);
  static void makeSimple(Graph* graph, std::vector<edge> &removed);
  static bool simpleTest(Graph *graph, std::vector<edge> *multipleEdges=0, std::vector<edge> *loops=0);
 private:
  SimpleTest();
  void addEdge(Graph *, const edge);
  void delEdge(Graph *, const edge);
  void destroy(Graph *);
  void deleteResult(Graph *graph);
  static SimpleTest *instance;
  TLP_HASH_MAP<unsigned long, bool> resultsBuffer;
};
/*@}*/

}
#endif
