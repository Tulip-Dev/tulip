#include <iostream>
#include <tulip/TlpTools.h>

/**
 * Tutorial 001
 *
 * Create a graph 
 * add node and edges
 * remove an edge and a node
 */

using namespace std;

int main() {
  //create an empty graph
  SuperGraph *graph=tlp::newSuperGraph();
  //add three nodes
  node n1=graph->addNode();
  node n2=graph->addNode();
  node n3=graph->addNode();
  //add three edges
  edge e=graph->addEdge(n2,n3);
  graph->addEdge(n1,n2);
  graph->addEdge(n3,n1);
  //remove an edge
  graph->delEdge(e);
  //remove a node
  graph->delNode(n2);
  //print the result
  cout << graph << flush ;
  delete graph;
  return EXIT_SUCCESS;
}
