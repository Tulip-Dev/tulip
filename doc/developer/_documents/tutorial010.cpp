#include <iostream>
#include <tulip/TlpTools.h>
#include <tulip/Graph.h>
#include <String>

/**
 * Tutorial 010
 *
 * Create a graph and apply an algorithm on it
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
  
  //add the edge
  edge e0 = graph->addEdge(n0,n1);
 
	//run the algorithm
	String errMsg;
	DataSet dataSet
	PluginProgress pluginProgress;
	bool result = graph->applyPropertyAlgorithm("Bubble Pack", _layout, errMsg, pluginProgress, &dataSet);

	if (!result)
		cout << errMsg << endl;
	else
		cout << "Algorithm successfully applied" << endl;

  delete graph;
  return EXIT_SUCCESS;
}

