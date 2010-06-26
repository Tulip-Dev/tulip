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
 * Tutorial 006
 *
 * Create a graph
 * Order the edge around the nodes
 */

using namespace std;
using namespace tlp;

void buildGraph(Graph *graph) {
  //add three nodes
  node n1=graph->addNode();
  node n2=graph->addNode();
  node n3=graph->addNode();
  //add three edges
  graph->addEdge(n2,n3);
  graph->addEdge(n1,n2);
  graph->addEdge(n3,n1);
}

void displayAdjacency(node n, Graph *graph) {
  Iterator<edge>*ite=graph->getInOutEdges(n);
  while(ite->hasNext())
    cout << ite->next().id << " ";
  delete ite;
  cout << endl;
}

int main() {
  //create an empty graph
  Graph *graph=tlp::newGraph();

  //build the graph
  node n1=graph->addNode();
  node n2=graph->addNode();
  node n3=graph->addNode();
  node n4=graph->addNode();

  //add three edges
  edge e1=graph->addEdge(n2,n3);
  edge e2=graph->addEdge(n1,n2);
  edge e3=graph->addEdge(n3,n1);
  edge e4=graph->addEdge(n4,n1);

  //display current order of edge around n1
  displayAdjacency(n1,graph);
  
  //change e2 et e4
  graph->swapEdgeOrder(n1,e2,e4);

  //display the new order of edge around n1
  displayAdjacency(n1,graph);

  vector<edge> tmp(2);
  tmp[0]=e2;
  tmp[1]=e4;
  graph->setEdgeOrder(n1,tmp);
  //display the new order of edge around n1
  displayAdjacency(n1,graph);

  delete graph;
  return EXIT_SUCCESS;
}
