#include <iostream>
#include <tulip/Graph.h>

/**
 * Tutorial 003
 *
 * Create a graph and three sub graphq 
 * display all the structure using iterators
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

int main() {
  //create an empty graph
  Graph *graph=tlp::newGraph();

  //build the graph
  buildGraph(graph);

  //build two empty subgraph
  Graph *subgraph0=graph->addSubGraph();
  Graph *subgraph1=graph->addSubGraph();
  Graph *subgraph2=subgraph1->addSubGraph();

  //add node inside subgraphs
  buildGraph(subgraph0);
  buildGraph(subgraph1);
  buildGraph(subgraph2);

  //iterate subgraph (0 and 1 normally ) and output them
  Iterator<Graph *> *itS=graph->getSubGraphs();
  while (itS->hasNext()) 
    cout << itS->next() << endl;
  delete itS;

  delete graph;
  return EXIT_SUCCESS;
}
