#include <iostream>
#include <tulip/TlpTools.h>
#include <tulip/Graph.h>
#include <tulip/ForEach.h>

/**
 * Tutorial 002
 *
 * Create a graph 
 * display all the structure using forEach
 *
 */

using namespace std;
using namespace tlp;

void buildGraph(Graph *graph) {
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
	//initialize the Tulip libs
  initTulipLib(NULL);

  //create an empty graph
  Graph *graph = newGraph();

  //build the graph
  buildGraph(graph);

	bool first = true;
  
  //===========================
  //go through all nodes and display the structure
	node n;
	forEach(n, graph->getNodes()) {
    cout << "node: " <<  n.id << endl;
    cout << " degree: " << graph->deg(n) << endl;
    cout << " in-degree: " << graph->indeg(n) << endl;
    cout << " out-degree: " << graph->outdeg(n) << endl;

    //===========================
    //go through all ancestors of a node
    cout << " predecessors: {";
		node in_node;
    forEach(in_node, graph->getInNodes(n)) {
      if (!first) {cout << ",";} else {first = false;}
      cout << in_node.id;
    }
    cout << "}" << endl;
		first = true;

    //===========================
    //go through all successors of a node
    cout << " successors: {";
    node out_node;
    forEach(out_node, graph->getOutNodes(n)) {
      if (!first) {cout << ",";} else {first = false;}
      cout << out_node.id;
    }
    cout << "}" << endl;
		first = true;

    //===========================
    //go through the neighborhood of a node
    cout << " neighborhood: {";
		node in_out_node;
    forEach(in_out_node, graph->getInOutNodes(n)) {
			if (!first) {cout << ",";} else {first = false;}
      cout << in_out_node.id;
    } 
    cout << "}" << endl;
		first = true;

    //===========================
    //go through the incoming edges
    cout << " incoming edges: {";
    edge in_edge;
		forEach(in_edge, graph->getInEdges(n)) {
			if (!first) {cout << ",";} else {first = false;}
      cout << in_edge.id;
    }
    cout << "}" << endl;
		first = true;

    //===========================
    //go through the outcoming edges
    cout << " outcoming edges: {";
    edge out_edge;
		forEach(out_edge, graph->getOutEdges(n)) {
			if (!first) {cout << ",";} else {first = false;}
      cout << out_edge.id;
    }
    cout << "}" << endl;
		first = true;

    //===========================
    //go through the adjacent edges
    cout << " adjacent edges: {";
    edge in_out_edge;
		forEach(in_out_edge, graph->getInOutEdges(n)) {
			if (!first) {cout << ",";} else {first = false;}
      cout << in_out_edge.id;
    }
    cout << "}" << endl;
		first = true;
  }

  //===========================
  //go through all edges
	edge e;
  forEach(e, graph->getEdges()) {
    cout << "edge: " << e.id;
    cout << " source: " << graph->source(e).id;
    cout << " target: " << graph->target(e).id;
    cout << endl;
  } 

  delete graph; //delete the entire graph
  return EXIT_SUCCESS;
}
