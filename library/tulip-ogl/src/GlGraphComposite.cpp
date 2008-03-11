#include "tulip/GlGraphComposite.h"

#include <iostream>

#include <tulip/GraphProperty.h>
#include <tulip/ForEach.h>

#include "tulip/GlTools.h"
#include "tulip/GlDisplayListManager.h"

using namespace std;

namespace tlp {

  GlGraphComposite::GlGraphComposite(Graph* graph):inputData(graph,&parameters) {
    graph->addObserver(this);
    
    buildLists();
  }
  void GlGraphComposite::buildLists() {
    if(!nodes.empty())
      nodes.clear();
    if(!metaNodes.empty())
      metaNodes.clear();
    if(!edges.empty())
      edges.clear();

    Graph *graph=inputData.getGraph();
    if (parameters.isElementOrdered()) {
      list<node> orderedNode;
      list<edge> orderedEdge;
      DoubleProperty *metric = graph->getProperty<DoubleProperty>("viewMetric");
      node n;
      forEach(n, graph->getNodes())
	orderedNode.push_back(n);
      LessThanNode comp;
      comp.metric=metric;
      orderedNode.sort(comp);
      edge e;
      forEach(e, graph->getEdges()) 
	orderedEdge.push_back(e);
      LessThanEdge comp2;
      comp2.metric = metric;
      comp2.sp = graph;
      orderedEdge.sort(comp2);
      
      for(list<node>::iterator itN=orderedNode.begin();itN!=orderedNode.end();++itN) {
	if(inputData.elementGraph->getNodeValue(*itN) == 0){
	  nodes.push_back(GlNode((*itN).id));
	}else{
	  metaNodes.push_back(GlMetaNode((*itN).id));
	}
      }
      
      for(list<edge>::iterator itE=orderedEdge.begin();itE!=orderedEdge.end();++itE) {
	edges.push_back(GlEdge((*itE).id));
      }
      
    } else {
      Iterator<node>* drawNodesIterator = graph->getNodes();
      Iterator<edge>* drawEdgesIterator = graph->getEdges();
      
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
  }
  //===================================================================
  const GlGraphRenderingParameters& GlGraphComposite::getRenderingParameters() {
    return parameters;
  }
  //===================================================================
  void GlGraphComposite::setRenderingParameters(const GlGraphRenderingParameters &parameter) {
    if(parameters.isElementOrdered() != parameter.isElementOrdered()) {
      parameters = parameter;
      buildLists();
    }else{
      parameters = parameter;
    }
  }
  //===================================================================
  GlGraphInputData* GlGraphComposite::getInputData() {
    return &inputData;
  }
  //===================================================================
  void GlGraphComposite::addNode(Graph *graph,const node n) {
    nodesToAdd.push_back(n.id);
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
	break;
      }
    }
    for(vector<unsigned int>::iterator it=nodesToAdd.begin();it!=nodesToAdd.end();++it) {
      if((*it)==n.id) {
	nodesToAdd.erase(it);
	break;
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
    inputData.graph=NULL;
  }
  //===================================================================
  void GlGraphComposite::addNodes() {
    if(!nodesToAdd.empty()) {
      for(vector<unsigned int>::iterator it=nodesToAdd.begin();it!=nodesToAdd.end();++it) {
	if(inputData.elementGraph->getNodeValue(node(*it))==0){
	  nodes.push_back(GlNode(*it)); 
	}
	else{
	  metaNodes.push_back(GlMetaNode(*it));
	}
      }
      nodesToAdd.clear();
    }
  }
  //====================================================
  void GlGraphComposite::getXML(xmlNodePtr rootNode){
    GlXMLTools::createProperty(rootNode, "type", "GlGraphComposite");
  }
   //====================================================
  void GlGraphComposite::setWithXML(xmlNodePtr rootNode){
  }
}
