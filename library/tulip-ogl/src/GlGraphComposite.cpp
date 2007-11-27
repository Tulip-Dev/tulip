#include "tulip/GlGraphComposite.h"

#include <iostream>

#include <tulip/GraphProperty.h>

#include "tulip/GlTools.h"
#include "tulip/GlDisplayListManager.h"

using namespace std;

namespace tlp {

  GlGraphComposite::GlGraphComposite(Graph* graph):inputData(graph,&parameters) {
    graph->addObserver(this);
    Iterator<node>* drawNodesIterator;
    Iterator<edge>* drawEdgesIterator;
    if (parameters.isElementOrdered()) {
      cout << "Error GlGraphComposite ordered list" << endl;
      assert(false);
    }
    else {
      drawNodesIterator = graph->getNodes();
      drawEdgesIterator = graph->getEdges();
    }

    if (!drawNodesIterator->hasNext() || graph->numberOfNodes()==0) return;

    unsigned int number = graph->numberOfNodes(); 

    while ((drawNodesIterator->hasNext()) && (number >0)) {
      --number;
      unsigned int id=drawNodesIterator->next().id;
      if(inputData.elementGraph->getNodeValue(node(id)) == 0){
	nodes.push_back(GlNode(id));
      }else{
	metaNodes.push_back(GlMetaNode(id));
      }
    }

    number = graph->numberOfEdges(); 

    while ((drawEdgesIterator->hasNext()) && (number >0)) {
      --number;
      edges.push_back(GlEdge(drawEdgesIterator->next().id));
    }
  }
  //===================================================================
  const GlGraphRenderingParameters& GlGraphComposite::getRenderingParameters() {
    return parameters;
  }
  //===================================================================
  void GlGraphComposite::setRenderingParameters(const GlGraphRenderingParameters &parameter) {
    parameters = parameter;
  }
  //===================================================================
  GlGraphInputData* GlGraphComposite::getInputData() {
    return &inputData;
  }
  //===================================================================
  void GlGraphComposite::addNode(Graph *graph,const node n) {
    nodes.push_back(GlNode(n.id)); 
  }
  //===================================================================
  void GlGraphComposite::addEdge(Graph *graph,const edge e) {
    edges.push_back(GlEdge(e.id)); 
  }
  //===================================================================
  void GlGraphComposite::delNode(Graph *graph,const node n) {
    for(vector<GlNode>::iterator it=nodes.begin();it!=nodes.end();++it) {
      if((*it).id==n.id) {
	nodes.erase(it);
	return;
      }
    }
  }
  //===================================================================
  void GlGraphComposite::delEdge(Graph *graph,const edge e) {
    for(vector<GlEdge>::iterator it=edges.begin();it!=edges.end();++it) {
      if((*it).id==e.id) {
	edges.erase(it);
	return;
      }
    }
  }
  //===================================================================
  void GlGraphComposite::destroy(Graph *graph) {
    nodes.clear();
    edges.clear();
  }
}
