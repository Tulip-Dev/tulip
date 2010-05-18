#include "tulip/GlGraphComposite.h"

#include <iostream>

#include <tulip/ForEach.h>
#include <tulip/GraphProperty.h>

#include "tulip/GlTools.h"
#include "tulip/GlDisplayListManager.h"

using namespace std;

namespace tlp {

  GlGraphComposite::GlGraphComposite(Graph* graph):inputData(graph,&parameters),rootGraph(graph->getRoot()),haveToSort(true),nodesModified(true) {
    graph->addGraphObserver(this);
    graph->getProperty<GraphProperty>("viewMetaGraph")->addPropertyObserver(this);

    Iterator<node>* nodesIterator = graph->getNodes();
    while (nodesIterator->hasNext()){
      node n=nodesIterator->next();
      if(graph->getNodeMetaInfo(n))
        metaNodes.insert(n);
    }
    delete nodesIterator;
  }

  GlGraphComposite::~GlGraphComposite(){
    inputData.getGraph()->removeGraphObserver(this);
    inputData.getGraph()->getProperty<GraphProperty>("viewMetaGraph")->removePropertyObserver(this);
  }

  void GlGraphComposite::acceptVisitorForNodes(Graph *graph,GlSceneVisitor *visitor){
    if(isDisplayNodes() || isDisplayMetaNodes()){
      visitor->reserveMemoryForNodes(graph->numberOfNodes());
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
      delete nodesIterator;
    }
  }

  void GlGraphComposite::acceptVisitorForEdges(Graph *graph,GlSceneVisitor *visitor){
    if(isDisplayEdges() || parameters.isViewEdgeLabel()) {
      visitor->reserveMemoryForEdges(graph->numberOfEdges());

      GlEdge glEdge(0);
      Iterator<edge>* edgesIterator = graph->getEdges();
      while (edgesIterator->hasNext()){
        glEdge.id=edgesIterator->next().id;
        glEdge.acceptVisitor(visitor);
      }
      delete edgesIterator;
    }
  }

  void GlGraphComposite::acceptVisitor(GlSceneVisitor *visitor)
  {
    Graph *graph=inputData.getGraph();

    // Check if the current graph are in the hierarchy
    assert((rootGraph==graph) || (rootGraph->isDescendantGraph(graph)));

    if(visitor->isThreadSafe()){
#ifdef HAVE_OMP
#pragma omp parallel
#endif
      {
#ifdef HAVE_OMP
#pragma omp sections nowait
#endif
        {
          acceptVisitorForNodes(graph,visitor);
        }
#ifdef HAVE_OMP
#pragma omp sections nowait
#endif
        {
          acceptVisitorForEdges(graph,visitor);
        }
      }
    }else{
      acceptVisitorForNodes(graph,visitor);
      acceptVisitorForEdges(graph,visitor);
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
