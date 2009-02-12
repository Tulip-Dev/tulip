#include "tulip/GlGraphComposite.h"

#include <iostream>

#include <tulip/ForEach.h>

#include "tulip/GlTools.h"
#include "tulip/GlDisplayListManager.h"

using namespace std;

namespace tlp {

  GlGraphComposite::GlGraphComposite(Graph* graph):inputData(graph,&parameters),haveToSort(true) {
    graph->addGraphObserver(this);
  }

  void GlGraphComposite::acceptVisitor(GlSceneVisitor *visitor)
  {
    Graph *graph=inputData.getGraph();

    if(!parameters.isElementOrdered()){
      if(isDisplayNodes() || isDisplayMetaNodes()){
        GlNode glNode(0);
        bool isMetaNode;

        Iterator<node>* nodesIterator = graph->getNodes();
        while (nodesIterator->hasNext()){
          node n=nodesIterator->next();
          isMetaNode = inputData.getGraph()->isMetaNode(n);
          if((isDisplayNodes() && !isMetaNode) || (isDisplayMetaNodes() && isMetaNode)){
            glNode.id=n.id;
            glNode.acceptVisitor(visitor);
          }
        }
      }

      if(isDisplayEdges() || parameters.isViewEdgeLabel()) {
        GlEdge glEdge(0);
        Iterator<edge>* edgesIterator = graph->getEdges();
        while (edgesIterator->hasNext()){
          glEdge.id=edgesIterator->next().id;
          glEdge.acceptVisitor(visitor);
        }
      }
    }else{
      if(haveToSort)
        buildSortedList();

      if(isDisplayNodes() || isDisplayMetaNodes()){
        GlNode glNode(0);
        bool isMetaNode;

        for(list<node>::iterator it=sortedNodes.begin();it!=sortedNodes.end();++it){
          isMetaNode = inputData.getGraph()->isMetaNode(*it);
          if((isDisplayNodes() && !isMetaNode) || (isDisplayMetaNodes() && isMetaNode)){
            glNode.id=(*it).id;
            glNode.acceptVisitor(visitor);
          }
        }
      }

      if(isDisplayEdges() || parameters.isViewEdgeLabel()) {
        GlEdge glEdge(0);
        for(list<edge>::iterator it=sortedEdges.begin();it!=sortedEdges.end();++it){
          glEdge.id=(*it).id;
          glEdge.acceptVisitor(visitor);
        }
      }
    }
  }

  GlGraphComposite::~GlGraphComposite(){
  }
  void GlGraphComposite::buildSortedList() {
    haveToSort=false;

    DoubleProperty *metric = inputData.getGraph()->getProperty<DoubleProperty>("viewMetric");
    node n;
    forEach(n, inputData.getGraph()->getNodes())
      sortedNodes.push_back(n);
    LessThanNode comp;
    comp.metric=metric;
    sortedNodes.sort(comp);
    edge e;
    forEach(e, inputData.getGraph()->getEdges())
      sortedEdges.push_back(e);
    LessThanEdge comp2;
    comp2.metric = metric;
    comp2.sp = inputData.getGraph();
    sortedEdges.sort(comp2);
  }
  //===================================================================
  const GlGraphRenderingParameters& GlGraphComposite::getRenderingParameters() {
    return parameters;
  }
  //===================================================================
  void GlGraphComposite::setRenderingParameters(const GlGraphRenderingParameters &parameter) {
    if(parameters.isElementOrdered() != parameter.isElementOrdered()) {
      parameters = parameter;
      haveToSort=true;
    }else{
      parameters = parameter;
    }
  }
  //===================================================================
  GlGraphInputData* GlGraphComposite::getInputData() {
    return &inputData;
  }
  //====================================================
  void GlGraphComposite::getXML(xmlNodePtr rootNode){
    GlXMLTools::createProperty(rootNode, "type", "GlGraphComposite");
  }
   //====================================================
  void GlGraphComposite::setWithXML(xmlNodePtr rootNode){
  }
}
