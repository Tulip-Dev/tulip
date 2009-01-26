#include "tulip/GlGraphComposite.h"

#include <iostream>

#include <tulip/ForEach.h>

#include "tulip/GlTools.h"
#include "tulip/GlDisplayListManager.h"

using namespace std;

namespace tlp {

  GlGraphComposite::GlGraphComposite(Graph* graph):inputData(graph,&parameters) {
    graph->addGraphObserver(this);

    buildLists();
  }

  GlGraphComposite::~GlGraphComposite(){
    if(inputData.graph)
      inputData.graph->removeGraphObserver(this);
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
	if(inputData.getGraph()->isMetaNode(*itN)){
	  metaNodes.insert(GlMetaNode((*itN).id));
	}else{
	  nodes.insert(GlNode((*itN).id));
	}
      }

      for(list<edge>::iterator itE=orderedEdge.begin();itE!=orderedEdge.end();++itE) {
	edges.insert(GlEdge((*itE).id));
      }

    } else {
      Iterator<node>* drawNodesIterator = graph->getNodes();
      Iterator<edge>* drawEdgesIterator = graph->getEdges();

      if (!drawNodesIterator->hasNext() || graph->numberOfNodes()==0) return;

      unsigned int number = graph->numberOfNodes();

      while ((drawNodesIterator->hasNext()) && (number >0)) {
	--number;
	unsigned int id=drawNodesIterator->next().id;
	if(inputData.getGraph()->isMetaNode(node(id))){
	  metaNodes.insert(GlMetaNode(id));
	}else{
	  nodes.insert(GlNode(id));
	}
      }

      number = graph->numberOfEdges();

      while ((drawEdgesIterator->hasNext()) && (number >0)) {
	--number;
	edges.insert(GlEdge(drawEdgesIterator->next().id));
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
    edges.insert(GlEdge(e.id));
  }
  //===================================================================
  void GlGraphComposite::delNode(Graph *graph,const node n) {
    set<GlMetaNode>::iterator it1=metaNodes.find(GlMetaNode(n.id));
    if(it1!=metaNodes.end())
      metaNodes.erase(it1);

    set<GlNode>::iterator it2=nodes.find(GlNode(n.id));
    if(it2!=nodes.end())
      nodes.erase(it2);

    for(vector<unsigned int>::iterator it=nodesToAdd.begin();it!=nodesToAdd.end();++it) {
      if((*it)==n.id) {
        nodesToAdd.erase(it);
        break;
      }
    }
  }
  //===================================================================
  void GlGraphComposite::delEdge(Graph *graph,const edge e) {
    edges.erase(GlEdge(e.id));
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
	if(inputData.getGraph()->isMetaNode(node(*it))){
	  metaNodes.insert(GlMetaNode(*it));
	}
	else{
	  nodes.insert(GlNode(*it));
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
