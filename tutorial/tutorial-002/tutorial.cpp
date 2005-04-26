#include <iostream>
#include <tulip/TlpTools.h>

/**
 * Tutorial 002
 *
 * Create a graph 
 * display all the structure using iterators
 */

using namespace std;
void buildGraph(SuperGraph *graph) {
  //add three nodes
  node n0=graph->addNode();
  node n1=graph->addNode();
  node n2=graph->addNode();
  //add three edges
  graph->addEdge(n1,n2);
  graph->addEdge(n0,n1);
  graph->addEdge(n2,n0);
}

int main() {
  //create an empty graph
  SuperGraph *graph=tlp::newSuperGraph();

  Iterator<node> *itNodes=graph->getNodes();
  while(itNodes->hasNext()) {
    node n=itNodes->next();
    cout << "node: " <<  n.id << endl;
  }delete itNodes;

  //build the graph
  buildGraph(graph);

  //Iterate all nodes and display the structure
  itNodes=graph->getNodes();
  while(itNodes->hasNext()) {
    node n=itNodes->next();
    cout << "node: " <<  n.id << endl;
    cout << " degree: " << graph->deg(n) << endl;
    cout << " in-degree: " << graph->indeg(n) << endl;
    cout << " out-degree: " << graph->outdeg(n) << endl;
    cout << " ancestors: {";
    //iterate all ancestors of a node
    Iterator<node> *itN=graph->getInNodes(n);
    while(itN->hasNext()) {
      cout << itN->next().id;
      if (itN->hasNext()) cout << ",";
    } delete itN;
    cout << "}" << endl;
    cout << " successors: {";
    //iterate all successors of a node
    itN=graph->getOutNodes(n);
    while (itN->hasNext()) {
      cout << itN->next().id;
      if (itN->hasNext()) cout << ",";
    } delete itN;
    cout << "}" << endl;
    cout << " neighborhood: {";
    //iterate the neighborhood of a node
    itN=graph->getInOutNodes(n);
    while(itN->hasNext()) {
      cout << itN->next().id;
      if (itN->hasNext()) cout << ",";
    } delete itN;
    cout << "}" << endl;
    cout << " incoming edges: {";
    //iterate the incoming edges
    Iterator<edge> *itE=graph->getInEdges(n);
    while(itE->hasNext()) {
      cout << itE->next().id;
      if (itE->hasNext()) cout << ",";
    } delete itE;
    cout << "}" << endl;
    cout << " outcoming edges: {";
    //iterate the out comming edges
    itE=graph->getOutEdges(n);
    while(itE->hasNext()) {
      cout << itE->next().id;
      if (itE->hasNext()) cout << ",";
    } delete itE;
    cout << "}" << endl;
    cout << " adjacent edges: {";
    //iterate the adjacent edges
    itE=graph->getInOutEdges(n);
    while(itE->hasNext()) {
      cout << itE->next().id;
      if (itE->hasNext()) cout << ",";
    } delete itE;
    cout << "}" << endl;

  }delete itNodes;

  //Iterate all edges
  Iterator<edge> *itEdges=graph->getEdges();
  while(itEdges->hasNext()) {
    edge e = itEdges->next(); 
    cout << "edge: " << e.id;
    cout << " source: " << graph->source(e).id;
    cout << " target: " << graph->target(e).id;
    cout << endl;
  }delete itEdges;

  delete graph;
  return EXIT_SUCCESS;
}
