#include <iostream>
#include <tulip/MetaGraphProxy.h>
#include <tulip/TlpTools.h>

/**
 * Tutorial 004
 *
 * Create a graph and a properties of each type
 * And display properities present in the graph
 */

using namespace std;
void buildGraph(SuperGraph *graph) {
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
  SuperGraph *graph=tlp::newSuperGraph();
  //build the graph
  buildGraph(graph);

  //Get and create several properties
  MetricProxy *metric=graph->getLocalProperty<MetricProxy>("firstMetric");
  SelectionProxy *select=graph->getLocalProperty<SelectionProxy>("firstSelection");
  LayoutProxy *layout=graph->getLocalProperty<LayoutProxy>("firstLayout");
  IntProxy *integer=graph->getLocalProperty<IntProxy>("firstInteger");
  ColorsProxy *colors=graph->getLocalProperty<ColorsProxy>("firstColors");
  SizesProxy *sizes=graph->getLocalProperty<SizesProxy>("firstSizes");
  MetaGraphProxy *meta=graph->getLocalProperty<MetaGraphProxy>("firstMeta");
  StringProxy *strings=graph->getLocalProperty<StringProxy>("firstString");

  //initialize all the properties
  metric->setAllNodeValue(0.0);
  metric->setAllEdgeValue(0.0);
  select->setAllNodeValue(false);
  select->setAllEdgeValue(false);
  layout->setAllNodeValue(Coord(0,0,0)); //coordinates
  layout->setAllEdgeValue(vector<Coord>());//Vector of bends
  integer->setAllNodeValue(0);
  integer->setAllEdgeValue(0);
  sizes->setAllNodeValue(Size(0,0,0)); //width, height, depth
  sizes->setAllEdgeValue(Size(0,0,0)); //start_size, end_size, arrow_size
  colors->setAllNodeValue(Color(0,0,0,0));//Red, green, blue
  colors->setAllEdgeValue(Color(0,0,0,0));//Red, green, blue
  strings->setAllNodeValue("first");
  strings->setAllEdgeValue("first");
  meta->setAllNodeValue(graph); //an existing graph
  cout << "List of the properties present in the graph:" << endl;
  Iterator<string> *it=graph->getLocalProperties();
  while (it->hasNext()) {
    cout << it->next() << endl;
  } delete it;
  
  delete graph;
  return EXIT_SUCCESS;
}
