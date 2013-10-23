#include <iostream>
#include <tulip/TlpTools.h>
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
	//initialize the Tulip libs
  initTulipLib(NULL);

  //create an empty graph
  Graph *graph = newGraph();

  //add three nodes
  node n1 = graph->addNode();
  node n2 = graph->addNode();
  node n3 = graph->addNode();

  //add three edges
  edge e1 = graph->addEdge(n2,n3);
  edge e2 = graph->addEdge(n1,n2);
  edge e3 = graph->addEdge(n3,n1);

  //delete an edge
  graph->delEdge(e1);

  //delete a node
  graph->delNode(n2);

  //print the result on the standard output
  cout << graph << flush ;

  tlp::saveGraph(graph,"tutorial001.tlp");

  //delete the graph
  delete graph;
  return EXIT_SUCCESS;
}
