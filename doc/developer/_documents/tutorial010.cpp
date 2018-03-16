#include <iostream>
#include <tulip/TlpTools.h>
#include <tulip/Graph.h>
#include <tulip/LayoutProperty.h>

/**
 * Tutorial 010
 *
 * Create a graph and apply an algorithm on it
 *
 */

using namespace std;
using namespace tlp;

int main() {
  // initialize the Tulip libs
  initTulipLib();

  // create an empty graph
  Graph *graph = newGraph();

  // add the nodes
  node n0 = graph->addNode();
  node n1 = graph->addNode();

  // add the edge
  graph->addEdge(n0, n1);

  // run the algorithm
  string errMsg;
  LayoutProperty *layout = graph->getProperty<LayoutProperty>("viewLayout");
  bool result = graph->applyPropertyAlgorithm("Bubble Pack", layout, errMsg);

  if (!result)
    cout << errMsg << endl;
  else
    cout << "Algorithm successfully applied" << endl;

  delete graph;
  return EXIT_SUCCESS;
}
