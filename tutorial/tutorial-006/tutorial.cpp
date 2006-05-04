#include <iostream>
#include <tulip/GraphProperty.h>
#include <tulip/TlpTools.h>

/**
 * Tutorial 006
 *
 * Create a graph
 * Order the edge around the nodes
 */

using namespace std;
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
