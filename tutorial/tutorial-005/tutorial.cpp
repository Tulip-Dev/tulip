#include <iostream>
#include <tulip/BooleanProperty.h>
#include <tulip/ColorProperty.h>
#include <tulip/DoubleProperty.h>

/**
 * Tutorial 005
 *
 * Create a graph hierarchy and sevral  properties
 * Display the inherited and local properties in each graph
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

  //Get and create several properties
  BooleanProperty *select=graph->getLocalProperty<BooleanProperty>("firstSelection");
  graph->getLocalProperty<ColorProperty>("firstColors");
  graph->getLocalProperty<DoubleProperty>("firstMetric");

  //init the selection in order to use it for building clone subgraph
  select->setAllNodeValue(true);
  select->setAllEdgeValue(true);

  //Create a hierarchy of subgraph (there are all the same)
  Graph *subgraph1=graph->addSubGraph(select);
  Graph *subgraph2=subgraph1->addSubGraph(select);

  //create a property in subgraph1 (redefinition of the one defined in graph)
  subgraph1->getLocalProperty<DoubleProperty>("firstMetric");

  //create a new property in subgraph1
  subgraph1->getLocalProperty<DoubleProperty>("secondMetric");

  //create a new property in subgraph3
  subgraph2->getLocalProperty<DoubleProperty>("thirdMetric");

  cout << "List of the local properties present in the subgraph1:" << endl;
  Iterator<string> *it=subgraph1->getLocalProperties();
  while (it->hasNext()) {
    cout << it->next() << endl;
  } delete it;

  cout << "List of inherited properties present in the subgraph1:" << endl;
  it=subgraph1->getInheritedProperties();
  while (it->hasNext()) {
    cout << it->next() << endl;
  } delete it;

  cout << "List of the local properties present in the subgraph2:" << endl;
  it=subgraph2->getLocalProperties();
  while (it->hasNext()) {
    cout << it->next() << endl;
  } delete it;

  cout << "List of inherited properties present in the subgraph2:" << endl;
  it=subgraph2->getInheritedProperties();
  while (it->hasNext()) {
    cout << it->next() << endl;
  } delete it;

  delete graph;
  return EXIT_SUCCESS;
}
