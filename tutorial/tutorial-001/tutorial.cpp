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
#include <iostream>
#include <tulip/Graph.h>

/**
 *
 * Tutorial 001
 *
 * Create a graph 
 * add three nodes and three edges
 * remove an edge and a node
 * print the result on the standard output
 * 
 */

using namespace std;
using namespace tlp;

int main() {
  //create an empty graph
  Graph *graph = tlp::newGraph();

  //add three nodes
  node n1 = graph->addNode();
  node n2 = graph->addNode();
  node n3 = graph->addNode();

  //add three edges
  edge e1 = graph->addEdge(n2,n3);
  edge e2 = graph->addEdge(n1,n2);
  edge e3 = graph->addEdge(n3,n1);

  //remove an edge
  graph->delEdge(e1);

  //remove a node
  graph->delNode(n2);

  //print the result on the standard output
  cout << graph << flush ;

  //delete the graph
  delete graph;
  return EXIT_SUCCESS;
}
