/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 *
 * Tulip is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Tulip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */
#include <iostream>
#include <tulip/Graph.h>

/**
 * Tutorial 002
 *
 * Create a graph 
 * display all the structure using iterators
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
  //create an empty graph
  Graph *graph=tlp::newGraph();

  //build the graph
  buildGraph(graph);
  
  //===========================
  //Iterate all nodes and display the structure
  Iterator<node> *itNodes = graph->getNodes();
  while(itNodes->hasNext()) {
    node n = itNodes->next();
    cout << "node: " <<  n.id << endl;
    cout << " degree: " << graph->deg(n) << endl;
    cout << " in-degree: " << graph->indeg(n) << endl;
    cout << " out-degree: " << graph->outdeg(n) << endl;

    //===========================
    //iterate all ancestors of a node
    cout << " ancestors: {";
    Iterator<node> *itN=graph->getInNodes(n);
    while(itN->hasNext()) {
      cout << itN->next().id;
      if (itN->hasNext()) cout << ",";
    } delete itN; //!!!Warning : do not forget to delete iterators (memory leak)
    cout << "}" << endl;

    //===========================
    //iterate all successors of a node
    cout << " successors: {";
    itN = graph->getOutNodes(n);
    while (itN->hasNext()) {
      cout << itN->next().id;
      if (itN->hasNext()) cout << ",";
    } delete itN; //!!!Warning : do not forget to delete iterators (memory leak)
    cout << "}" << endl;

    //===========================
    //iterate the neighborhood of a node
    cout << " neighborhood: {";
    itN = graph->getInOutNodes(n);
    while(itN->hasNext()) {
      cout << itN->next().id;
      if (itN->hasNext()) cout << ",";
    } delete itN; //!!!Warning : do not forget to delete iterators (memory leak)
    cout << "}" << endl;

    //===========================
    //iterate the incoming edges
    cout << " incoming edges: {";
    Iterator<edge> *itE=graph->getInEdges(n);
    while(itE->hasNext()) {
      cout << itE->next().id;
      if (itE->hasNext()) cout << ",";
    } delete itE; //!!!Warning : do not forget to delete iterators (memory leak)
    cout << "}" << endl;
    cout << " outcoming edges: {";

    //===========================
    //iterate the outcomming edges
    itE = graph->getOutEdges(n);
    while(itE->hasNext()) {
      cout << itE->next().id;
      if (itE->hasNext()) cout << ",";
    } delete itE; //!!!Warning : do not forget to delete iterators (memory leak)
    cout << "}" << endl;

    //===========================
    //iterate the adjacent edges
    cout << " adjacent edges: {";
    itE = graph->getInOutEdges(n);
    while(itE->hasNext()) {
      cout << itE->next().id;
      if (itE->hasNext()) cout << ",";
    } delete itE; //!!!Warning : do not forget to delete iterators (memory leak)
    cout << "}" << endl;

  } delete itNodes; //!!!Warning : do not forget to delete iterators (memory leak)

  //===========================
  //Iterate all edges
  Iterator<edge> *itEdges=graph->getEdges();
  while(itEdges->hasNext()) {
    edge e = itEdges->next(); 
    cout << "edge: " << e.id;
    cout << " source: " << graph->source(e).id;
    cout << " target: " << graph->target(e).id;
    cout << endl;
  } delete itEdges; //!!!Warning : do not forget to delete iterators (memory leak)

  delete graph; //delete the entire graph
  return EXIT_SUCCESS;
}
