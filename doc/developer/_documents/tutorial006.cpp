#include <iostream>
#include <tulip/TlpTools.h>
#include <tulip/Graph.h>
#include <tulip/ForEach.h>
#include <tulip/MutableContainer.h>

/**
 * Tutorial 006
 *
 * Create a graph
 * Order the edges around the nodes
 */

using namespace std;
using namespace tlp;

void displayAdjacency(node n, Graph *graph) {
  edge e;
	forEach(e,graph->getInOutEdges(n))
    cout << e.id << " ";
  cout << endl;
}

int main() {
  //initialize the Tulip libs
  initTulipLib(NULL);

  //create an empty graph
  Graph *graph = newGraph();

  //add the nodes
  node n0 = graph->addNode();
  node n1 = graph->addNode();
  node n2 = graph->addNode();
  node n3 = graph->addNode();

  //add the edges
  edge e0 = graph->addEdge(n1,n2);
  edge e1 = graph->addEdge(n0,n1);
  edge e2 = graph->addEdge(n2,n0);
  edge e3 = graph->addEdge(n3,n0);

	//display current order of edge around n0
	cout << "Initial edge order around n0" << endl;
	displayAdjacency(n0, graph);

	//swap e2 and e3
	cout << "Swapping of e2 and e3" << endl;
  graph->swapEdgeOrder(n0, e2, e3);
	displayAdjacency(n0, graph);

	//reordering the edges
  vector<edge> tmp(2);
  tmp[0]=e2;
  tmp[1]=e3;
  graph->setEdgeOrder(n0,tmp);
	cout << "Reordering with a vector" << endl;
  displayAdjacency(n0,graph);

  delete graph;
  return EXIT_SUCCESS;
}
