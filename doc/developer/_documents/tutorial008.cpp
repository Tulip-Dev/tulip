#include <iostream>
#include <tulip/TlpTools.h>
#include <tulip/Graph.h>
#include <tulip/AcyclicTest.h>
#include <tulip/ConnectedTest.h>
#include <tulip/PlanarityTest.h>
#include <tulip/OuterPlanarTest.h>
#include <tulip/SimpleTest.h>
#include <tulip/TreeTest.h>

/**
 * Tutorial 008
 *
 * Create a graph and perform a few tests on it.
 * 
 */

using namespace std;
using namespace tlp;

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

	//run the tests
	if (AcyclicTest::isAcyclic(graph))
		cout << "The graph is acyclic" << endl;
	else
		cout << "The graph is cyclic" << endl;
    
	if (ConnectedTest::isConnected(graph))
		cout << "The graph is connected" << endl;
	else
		cout << "The graph is not connected" << endl;

	if (PlanarityTest::isPlanar(graph))
		cout << "The graph is planar" << endl;
	else
		cout << "The graph is not planar" << endl;

	if (SimpleTest::isSimple(graph))
		cout << "The graph is simple" << endl;
	else
		cout << "The graph is not simple" << endl;

	if (TreeTest::isTree(graph))
		cout << "The graph is a tree" << endl;
	else
		cout << "The graph is not a tree" << endl;


  delete graph;
  return EXIT_SUCCESS;
}
